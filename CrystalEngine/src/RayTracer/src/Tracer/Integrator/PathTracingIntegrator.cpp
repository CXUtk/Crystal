#include "PathTracingIntegrator.h"
#include <glm/gtx/transform.hpp>
#include <Function/Framework/Components/Material/BSDF.h>
#include <Core/Sampling/Sampling.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>

namespace tracer
{
	static constexpr float pRR = 0.8f;
    static constexpr float EPS = 1e-3;

	PathTracingIntegrator::PathTracingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth)
		: SamplerIntegrator(sampler, threads), m_maxDepth(maxDepth)
	{

	}


    glm::vec3 PathTracingIntegrator::Evaluate(const Ray3f& ray, const RayScene* scene,
            Sampler* sampler)
    {
        int bounces = 0;
        bool lightPath = true;
        Spectrum beta(1.f), L(0.f);
        Ray3f currentRay(ray);

        for (; bounces < m_maxDepth; bounces++)
        {
            SurfaceInteraction isec;

            bool foundIntersection = scene->Intersect(currentRay, &isec);

            Vector3f wo = -currentRay.Dir();
            Normal3f N = isec.GetNormal();
            Point3f P = isec.GetHitPos();

            // 如果是自发光物体就把发光项加上
            if (lightPath)
            {
                if (!foundIntersection)
                {
                    L += beta * scene->GetEnvironmentLight(currentRay.Dir());
                }
                else
                {
                    L += beta * isec.Le(-currentRay.Dir());
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
                currentRay = isec.SpawnRay(currentRay.Dir());
                continue;
            }

            // 计算从光源采样的radiance
            L += beta * UniformSampleAllLights(isec, scene, sampler);
            NAN_DETECT_V(L, "PathTracingIntegrator::L::UniformSampleAllLights");
            INF_DETECT_V(L, "PathTracingIntegrator::L::UniformSampleAllLights");

            // 进行一次路径追踪采样
            Vector3f wIn;
            float pdf;
            BxDFType type;

            Vector3f wOut = isec.ToLocalCoordinate(wo);
            auto brdf = bsdf->SampleDirection(sampler->Get1D(), sampler->Get2D(), wOut, &wIn,
                &pdf, BxDFType::BxDF_ALL, &type);

            NAN_DETECT_V(brdf, "PathTracingIntegrator::BSDF");
            INF_DETECT_V(brdf, "PathTracingIntegrator::BSDF");
            if (pdf == 0.f || brdf == Spectrum(0.f))
            {
                break;
            }

            beta *= bsdf->CalculateBSDFNoLDivideByPdf(wOut, wIn, type);
            Vector3f wi = isec.ToWorldCoordinate(wIn);

            bool specular = (type & BxDFType::BxDF_SPECULAR);
            lightPath = specular;
            currentRay = isec.SpawnRay(wi);

            //bool transmission = (type & BxDFType::BxDF_TRANSMISSION);
            //Float cosine = std::max(0.f, transmission ? glm::dot(-N, wIn) : glm::dot(N, wIn));

            //if (cosine == 0.f)
            //{
            //    beta *= 0.f;
            //}
            //else
            //{
            //    beta *= brdf * cosine / pdf;
            //}

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

	Spectrum PathTracingIntegrator::UniformSampleAllLights(const SurfaceInteraction& isec, const RayScene* scene,
        Sampler* sampler)
	{
        Spectrum L(0.f);
        // One sample for each light
        Vector2f sampleLight = sampler->Get2D();
        Vector2f sampleBSDF = sampler->Get2D();
        scene->ForEachLights([&](const crystal::Light* light) {
            if (light->Flux() == Spectrum(0.f)) return;
            L += EsimateDirect(isec, scene, sampleLight, sampleBSDF, light, sampler);
        });
        return L;
	}

    Spectrum PathTracingIntegrator::EsimateDirect(const SurfaceInteraction& isec, const RayScene* scene,
        const Vector2f& sampleLight, const Vector2f& sampleBSDF,
        const crystal::Light* light, Sampler* sampler)
    {
        Spectrum L(0.f);

        BxDFType bsdfSampleType = (BxDFType)(BxDFType::BxDF_ALL & ~BxDFType::BxDF_SPECULAR);
        Point3f P = isec.GetHitPos();
        Normal3f N = isec.GetNormal();
        Vector3f wOut = isec.ToLocalCoordinate(-isec.GetHitDir());

        auto bsdf = isec.GetBSDF();

        // Sample light source with MIS (Specular BSDF will not have value)
        {
            Point3f lightPos;
            float pdf_light;
            auto Li_light = light->Sample_Li(isec.GetSurfaceInfo(false), sampleLight, &lightPos, &pdf_light);

            Vector3f wIn = isec.ToLocalCoordinate(glm::normalize(lightPos - P));
            float NdotL = std::max(0.f, wIn.y);
            if (pdf_light != 0.f && !std::isinf(pdf_light)
                && Li_light != Spectrum(0.f) && NdotL != 0.f)
            {
                Spectrum f = bsdf->DistributionFunction(wOut, wIn);
                float pdf_bsdf = bsdf->Pdf(wOut, wIn, bsdfSampleType);
                if (f != Spectrum(0.f))
                {
                    if (scene->IntersectTest(isec.SpawnRayTo(lightPos), 0, 1.f - EPS))
                    {
                        Li_light = Spectrum(0.f);
                    }

                    if (Li_light != Spectrum(0.f))
                    {
                        if (light->IsDeltaLight())
                        {
                            L += f * Li_light * NdotL / pdf_light;
                        }
                        else
                        {
                            L += f * Li_light * NdotL * PowerHeuristic(1, pdf_light, 1, pdf_bsdf) / pdf_light;
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
            BxDFType sampledType;
            Vector3f wIn;
            Spectrum f = bsdf->SampleDirection(sampler->Get1D(), sampleBSDF, wOut, &wIn,
                &pdf_bsdf, bsdfSampleType, &sampledType);
            float NdotL = std::max(0.f, wIn.y);

            Vector3f wi = isec.ToWorldCoordinate(wIn);

            if (f == Spectrum(0.f) || pdf_bsdf == 0.f || NdotL == 0.f)
            {
                return L;
            }
            bool specularBSDF = sampledType & BxDFType::BxDF_SPECULAR;

            float weight = 1.0f;
            if (!specularBSDF)
            {
                Float pdf_light = light->Pdf_Li(isec.GetSurfaceInfo(false), wi);
                if (pdf_light == 0.f || f == Spectrum(0.f))
                {
                    return L;
                }
                weight = PowerHeuristic(1, pdf_bsdf, 1, pdf_light);
            }

            Spectrum Li(0.f);
            Ray lightTestRay = isec.SpawnRay(wi);
            SurfaceInteraction lightIsec;

            if (light->GetFlags() & LightFlags::Area)
            {
                if (scene->Intersect(lightTestRay, &lightIsec)
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
                if (!scene->IntersectTest(lightTestRay, 0, std::numeric_limits<float>::infinity()))
                {
                    Li = light->Le(wi);
                }
            }

            if (Li != Spectrum(0.f))
            {
                L += Li * bsdf->CalculateBSDFNoLDivideByPdf(wOut, wIn, sampledType) * weight;
            }
        }

        NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        return L;
    }
}