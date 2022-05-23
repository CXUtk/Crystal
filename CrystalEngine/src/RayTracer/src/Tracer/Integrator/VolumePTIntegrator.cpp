#include "VolumePTIntegrator.h"
#include <glm/gtx/transform.hpp>
#include <Function/Framework/Components/Material/BSDF.h>
#include <Core/Sampling/Sampling.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>

namespace tracer
{
	static constexpr float pRR = 0.8f;
    static constexpr float EPS = 1e-3;

    VolumePTIntegrator::VolumePTIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth)
		: SamplerIntegrator(sampler, threads), m_maxDepth(maxDepth)
	{

	}


    Spectrum VolumePTIntegrator::Evaluate(const RayTr& ray, const RayScene* scene,
            Sampler* sampler)
    {
        int bounces = 0;
        bool lightPath = true;
        Spectrum beta(1.f), L(0.f);
        RayTr currentRay(ray);

        for (; bounces < m_maxDepth; bounces++)
        {
            SurfaceInteraction isec;
            bool foundIntersection = scene->Intersect(currentRay.Ray, &isec);

            // Handle ray pass through a volume
            MediumInteractionInfo mi;
            if (currentRay.Medium)
            {
                // If doesn't sample to a medium, then mi.Medium = nullptr
                beta *= currentRay.Medium->Sample(currentRay.Ray, isec.GetDistance(), sampler, &mi);
            }
            if (beta == Spectrum(0.f))
            {
                break;
            }

            Vector3f wo = -currentRay.Ray.Dir();
            Normal3f N = isec.GetInteractionNormal();
            Point3f P = isec.GetPosition();

            // If sampled to a medium
            if (mi.GetMedium())
            {
                // Handle scattering at point in medium for volumetric path tracer
                L += beta * UniformSampleOneLight(mi, scene, sampler, true);
                NAN_DETECT_V(L, "VolumePTIntegrator::L::MediumPath");
                INF_DETECT_V(L, "VolumePTIntegrator::L::MediumPath");

                Vector3f wo = -currentRay.Ray.Dir(), wi;
                mi.GetPhaseFunction()->Sample_p(wo, &wi, sampler->Get2D());
                currentRay = mi.SpawnRay(wi);
                lightPath = false;
            }
            else
            {
                // 如果是自发光物体就把发光项加上
                if (lightPath)
                {
                    if (!foundIntersection)
                    {
                        L += beta * scene->GetEnvironmentLight(currentRay.Ray.Dir());
                    }
                    else
                    {
                        L += beta * isec.Le(-currentRay.Ray.Dir());
                    }
                }

                if (!foundIntersection)
                {
                    break;
                }

                isec.SetBSDF(std::make_shared<BSDF>(&isec));
                isec.GetMaterial()->ComputeScatteringFunctions(&isec, true);

                // No bsdf function, means the object is transparent, go through it
                auto bsdf = isec.GetBSDF();
                if (bsdf->IsEmpty())
                {
                    currentRay = isec.SpawnRay(currentRay.Ray.Dir());
                    continue;
                }

                // 计算从光源采样的radiance
                L += beta * UniformSampleAllLights(isec, scene, sampler, true);
                NAN_DETECT_V(L, "VolumePTIntegrator::L::UniformSampleAllLights");
                INF_DETECT_V(L, "VolumePTIntegrator::L::UniformSampleAllLights");

                // 进行一次路径追踪采样
                Vector3f wIn;
                float pdf;
                BxDFType type;

                Vector3f wOut = isec.ToLocalCoordinate(wo);
                auto brdf = bsdf->SampleDirection(sampler->Get1D(), sampler->Get2D(), wOut, &wIn,
                    &pdf, BxDFType::BxDF_ALL, &type);

                NAN_DETECT_V(brdf, "VolumePTIntegrator::BSDF");
                INF_DETECT_V(brdf, "VolumePTIntegrator::BSDF");
                if (pdf == 0.f || brdf == Spectrum(0.f))
                {
                    break;
                }

                beta *= bsdf->CalculateBSDFNoLDivideByPdf(wOut, wIn, type);
                Vector3f wi = isec.ToWorldCoordinate(wIn);

                bool specular = (type & BxDFType::BxDF_SPECULAR);
                lightPath = specular;
                currentRay = isec.SpawnRay(wi);
            }
            if (bounces > 3)
            {
                float q = std::max(.05f, 1.f - glm::length(beta));
                if (sampler->Get1D() < q)
                    break;
                beta /= 1 - q;
            }

            NAN_DETECT_V(L, "PathTracingIntegrator::L");
            INF_DETECT_V(L, "PathTracingIntegrator::L");

            NAN_DETECT_V(beta, "PathTracingIntegrator::beta");
            INF_DETECT_V(beta, "PathTracingIntegrator::beta");
        }
        return L;
    }

	Spectrum VolumePTIntegrator::UniformSampleAllLights(const InteractionInfo& it, const RayScene* scene,
        Sampler* sampler, bool handleMedium)
	{
        Spectrum L(0.f);
        // One sample for each light
        Vector2f sampleLight = sampler->Get2D();
        Vector2f sampleBSDF = sampler->Get2D();
        scene->ForEachLights([&](const crystal::Light* light) {
            if (light->Flux() == Spectrum(0.f)) return;
            L += EsimateDirect(it, scene, sampleLight, sampleBSDF, light, sampler, handleMedium);
        });
        return L;
	}

    Spectrum VolumePTIntegrator::UniformSampleOneLight(const InteractionInfo& it,
        const RayScene* scene, Sampler* sampler, bool handleMedium)
    {
        Float u = sampler->Get1D();
        Vector2f sampleLight = sampler->Get2D();
        Vector2f sampleBSDF = sampler->Get2D();

        Float pdf, remapped;
        auto light = scene->SampleOneLight(u, &pdf, &remapped);
        return EsimateDirect(it, scene, sampleLight, sampleBSDF, light, sampler, handleMedium) / pdf;
    }

    Spectrum VolumePTIntegrator::EsimateDirect(const InteractionInfo& it, const RayScene* scene,
        const Vector2f& sampleLight, const Vector2f& sampleBSDF,
        const crystal::Light* light, Sampler* sampler, bool handleMedium)
    {
        Spectrum L(0.f);
        BxDFType bsdfSampleType = (BxDFType)(BxDFType::BxDF_ALL & ~BxDFType::BxDF_SPECULAR);
        Vector3f wOut = it.ToLocalCoordinate(it.GetW_Out());
        Point3f P = it.GetPosition();

        // Sample light source with MIS (Specular BSDF will not have value)
        {
            Point3f lightPos;
            float pdf_light;
            auto Li_light = light->Sample_Li(it.GetGeometryInfo(), sampleLight,
                &lightPos, &pdf_light);
            Vector3f wIn = it.ToLocalCoordinate(glm::normalize(lightPos - P));
            
            if (pdf_light != 0.f && !std::isinf(pdf_light) && Li_light != Spectrum(0.f))
            {
                Spectrum f;
                Float pdf_bsdf;
                if (it.IsSurfaceInteraction())
                {
                    const SurfaceInteraction& isec = dynamic_cast<const SurfaceInteraction&>(it);
                    auto bsdf = isec.GetBSDF();

                    // For surface interaction, we need to account surface radiance
                    f = bsdf->DistributionFunction(wOut, wIn) * std::abs(wIn.y);
                    pdf_bsdf = bsdf->Pdf(wOut, wIn, bsdfSampleType);
                }
                else
                {
                    const MediumInteractionInfo& mi = dynamic_cast<const MediumInteractionInfo&>(it);
                    auto phase = mi.GetPhaseFunction();
                    f = phase->Pdf(wOut, wIn);
                    pdf_bsdf = phase->Pdf(wOut, wIn).x;
                }

                if (f != Spectrum(0.f))
                {
                    if (handleMedium)
                    {
                        Li_light *= TraceLi(scene, it, lightPos);
                    }
                    else
                    {
                        if (scene->IntersectTest(it.SpawnRayTo(lightPos).Ray, 0, 1.f - EPS))
                        {
                            Li_light = Spectrum(0.f);
                        }
                    }

                    if (Li_light != Spectrum(0.f))
                    {
                        if (light->IsDeltaLight())
                        {
                            L += f * Li_light / pdf_light;
                        }
                        else
                        {
                            L += f * Li_light * PowerHeuristic(1, pdf_light, 1, pdf_bsdf) / pdf_light;
                        }
                    }
                }

                NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
                INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
            }
        }

        // Sample BSDF (Delta light should not have any value in BSDF sample)
        if (!light->IsDeltaLight())
        {
            float pdf_bsdf = 0.f;
            Vector3f wIn;
            Spectrum f;
            BxDFType sampledType;
            bool sampledSpecular = false;
            if (it.IsSurfaceInteraction())
            {
                const SurfaceInteraction& isec = dynamic_cast<const SurfaceInteraction&>(it);
                auto bsdf = isec.GetBSDF();
                f = bsdf->SampleDirection(sampler->Get1D(), sampleBSDF, wOut, &wIn,
                    &pdf_bsdf, bsdfSampleType, &sampledType) * std::abs(wIn.y);
                sampledSpecular = sampledType & BxDFType::BxDF_SPECULAR;
            }
            else
            {
                const MediumInteractionInfo& mi = dynamic_cast<const MediumInteractionInfo&>(it);
                auto phase = mi.GetPhaseFunction();
                f = phase->Sample_p(wOut, &wIn, sampleBSDF);
                pdf_bsdf = phase->Pdf(wOut, wIn).x;
            }
            Vector3f wi = it.ToWorldCoordinate(wIn);
            if (f == Spectrum(0.f) || pdf_bsdf == 0.f)
            {
                return L;
            }

            float weight = 1.0f;
            if (!sampledSpecular)
            {
                Float pdf_light = light->Pdf_Li(it.GetGeometryInfo(), wi);
                if (pdf_light == 0.f || f == Spectrum(0.f))
                {
                    return L;
                }
                weight = PowerHeuristic(1, pdf_bsdf, 1, pdf_light);
            }

            Spectrum Li(0.f);
            RayTr lightTestRay = it.SpawnRay(wi);
            SurfaceInteraction lightIsec;

            if (handleMedium)
            {
                Spectrum Tr = Spectrum(1.0);
                if (light->GetFlags() & LightFlags::Area)
                {
                    if (scene->IntersectTr(lightTestRay, sampler, &lightIsec, &Tr)
                        && lightIsec.GetHitPrimitive()->GetAreaLight() == light)
                    {
                        Li = lightIsec.Le(-wi);
                    }
                    else
                    {
                        Li = light->Le(wi);
                    }
                }
                else if (light->GetFlags() & LightFlags::Infinite)
                {
                    if (!scene->IntersectTr(lightTestRay, sampler, &lightIsec, &Tr))
                    {
                        Li = light->Le(wi);
                    }
                }
                Li *= Tr;
            }
            else
            {
                if (light->GetFlags() & LightFlags::Area)
                {
                    if (scene->Intersect(lightTestRay.Ray, &lightIsec)
                        && lightIsec.GetHitPrimitive()->GetAreaLight() == light)
                    {
                        Li = lightIsec.Le(-wi);
                    }
                    else
                    {
                        Li = light->Le(wi);
                    }
                }
                else if (light->GetFlags() & LightFlags::Infinite)
                {
                    if (!scene->IntersectTest(lightTestRay.Ray, 0, std::numeric_limits<float>::infinity()))
                    {
                        Li = light->Le(wi);
                    }
                }
            }

            if (Li != Spectrum(0.f))
            {
                if (it.IsSurfaceInteraction())
                {
                    const SurfaceInteraction& isec = dynamic_cast<const SurfaceInteraction&>(it);
                    auto bsdf = isec.GetBSDF();
                    L += Li * bsdf->CalculateBSDFNoLDivideByPdf(wOut, wIn, sampledType) * weight;
                }
                else
                {
                    L += Li * f * weight / pdf_bsdf;
                }
            }
        }

        NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        return L;
    }

    //Spectrum VolumePTIntegrator::EsimateDirectMedium(const MediumInteractionInfo& mi, const RayScene* scene,
    //    const Vector2f& sampleLight, const Vector2f& sampleBSDF,
    //    const crystal::Light* light, Sampler* sampler, bool handleMedium)
    //{
    //    Spectrum L(0.f);
    //    BxDFType bsdfSampleType = (BxDFType)(BxDFType::BxDF_ALL & ~BxDFType::BxDF_SPECULAR);

    //    Point3f P = mi.GetPosition();

    //    Vector3f wOut = mi.ToLocalCoordinate(mi.GetW_Out());

    //    auto phase = mi.GetPhaseFunction();

    //    // Sample light source with MIS (Specular BSDF will not have value)
    //    {
    //        Point3f lightPos;
    //        float pdf_light;
    //        auto Li_light = light->Sample_Li(mi.GetGeometryInfo(), sampleLight,
    //            &lightPos, &pdf_light);

    //        Vector3f wIn = mi.ToLocalCoordinate(glm::normalize(lightPos - P));
    //        float NdotL = std::max(0.f, wIn.y);
    //        if (pdf_light != 0.f && !std::isinf(pdf_light)
    //            && Li_light != Spectrum(0.f) && NdotL != 0.f)
    //        {
    //            Spectrum f = phase->Pdf(wOut, wIn);
    //            float pdf_bsdf = phase->Pdf(wOut, wIn).x;
    //            if (f != Spectrum(0.f))
    //            {
    //                if (handleMedium)
    //                {
    //                    Li_light *= TraceLiM(scene, mi, lightPos);
    //                }

    //                if (Li_light != Spectrum(0.f))
    //                {
    //                    if (light->IsDeltaLight())
    //                    {
    //                        L += f * Li_light * NdotL / pdf_light;
    //                    }
    //                    else
    //                    {
    //                        L += f * Li_light * NdotL * PowerHeuristic(1, pdf_light, 1, pdf_bsdf) / pdf_light;
    //                    }
    //                }
    //            }

    //            NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
    //            INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
    //        }
    //    }

    //    // Sample BSDF (Delta light should not have any value in BSDF sample)
    //    if (!light->IsDeltaLight())
    //    {
    //        float pdf_bsdf = 0.f;
    //        BxDFType sampledType;
    //        Vector3f wIn;
    //        Spectrum f = phase->Sample_p(wOut, &wIn, sampler->Get2D());
    //        float NdotL = std::max(0.f, wIn.y);

    //        Vector3f wi = mi.ToWorldCoordinate(wIn);

    //        if (f == Spectrum(0.f) || pdf_bsdf == 0.f || NdotL == 0.f)
    //        {
    //            return L;
    //        }
    //        bool specularBSDF = sampledType & BxDFType::BxDF_SPECULAR;

    //        float weight = 1.0f;
    //        if (!specularBSDF)
    //        {
    //            Float pdf_light = light->Pdf_Li(mi.GetGeometryInfo(), wi);
    //            if (pdf_light == 0.f || f == Spectrum(0.f))
    //            {
    //                return L;
    //            }
    //            weight = PowerHeuristic(1, pdf_bsdf, 1, pdf_light);
    //        }

    //        Spectrum Li(0.f);
    //        RayTr lightTestRay = mi.SpawnRay(wi);
    //        SurfaceInteraction lightIsec;

    //        if (handleMedium)
    //        {
    //            Spectrum Tr = Spectrum(1.0);
    //            if (light->GetFlags() & LightFlags::Area)
    //            {
    //                if (scene->IntersectTr(lightTestRay, sampler, &lightIsec, &Tr)
    //                    && lightIsec.GetHitPrimitive()->GetAreaLight() == light)
    //                {
    //                    Li = lightIsec.Le(-wi);
    //                }
    //                else
    //                {
    //                    Li = light->Le(wi);
    //                }
    //            }
    //            else if (light->GetFlags() & LightFlags::Infinite)
    //            {
    //                if (!scene->IntersectTr(lightTestRay, sampler, &lightIsec, &Tr))
    //                {
    //                    Li = light->Le(wi);
    //                }
    //            }
    //            Li *= Tr;
    //        }

    //        if (Li != Spectrum(0.f))
    //        {
    //            L += Li * f * weight / pdf_bsdf;
    //        }
    //    }

    //    NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
    //    INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
    //    return L;
    //}

    Spectrum VolumePTIntegrator::TraceLi(const RayScene* scene, const InteractionInfo& it, const Point3f& lightPos)
    {
        Spectrum Tr(1.f);
        RayTr currentRay = it.SpawnRayTo(lightPos);
        while (true)
        {
            SurfaceInteraction isect;
            bool hitSurface = scene->Intersect(currentRay.Ray, &isect, 0.f, 1.f - EPS);

            // Hit a surface instead of a medium, then it is blocked
            if (hitSurface && isect.GetMaterial() != nullptr) return Spectrum(0.f);

            if (currentRay.Medium)
            {
                Tr *= currentRay.Medium->Tr(currentRay.Ray, isect.GetDistance(), nullptr);
            }

            if (!hitSurface) return Tr;

            currentRay = isect.SpawnRayTo(lightPos);
        }
        return Tr;
    }
}