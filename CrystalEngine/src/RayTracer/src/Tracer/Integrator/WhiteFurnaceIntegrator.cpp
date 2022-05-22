#include "WhiteFurnaceIntegrator.h"
#include <glm/gtx/transform.hpp>
#include <Function/Framework/Components/Material/BSDF.h>

#include <Core/Sampling/Sampling.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>

namespace tracer
{
    static constexpr float EPS = 1e-4;

    WhiteFurnaceIntegrator::WhiteFurnaceIntegrator(const std::shared_ptr<Sampler>& sampler,
		int threads) : SamplerIntegrator(sampler, threads)
	{}

    Spectrum WhiteFurnaceIntegrator::Evaluate(const RayTr& ray, const RayScene * scene, Sampler * sampler)
    {
        return eval_rec(ray, scene, sampler, 0, true);
    }

	Spectrum WhiteFurnaceIntegrator::eval_rec(const RayTr& ray, const RayScene* scene, Sampler* sampler,
		int level, bool specular)
	{
		Spectrum L(0.f);

		SurfaceInteraction isec;
		if (!scene->Intersect(ray.Ray, &isec))
		{
            return scene->GetEnvironmentLight(ray.Ray.Dir());
		}

		// Get emitted radiance from hit surface to such direction
		L += isec.Le(-ray.Ray.Dir());

		isec.SetBSDF(std::make_shared<BSDF>(&isec));
		isec.GetMaterial()->ComputeScatteringFunctions(&isec, true);

		L += UniformSampleAllLights(isec, scene, sampler);

        // Do one bsdf test
        Vector3f wo = -ray.Ray.Dir();
        Normal3f N = isec.GetInteractionNormal();
        Point3f P = isec.GetHitPos();
        Vector3f wIn;
        float pdf;
        BxDFType bxdfType;
        auto brdf = isec.GetBSDF()->SampleDirection(sampler->Get1D(), sampler->Get2D(), wo, &wIn,
            &pdf, BxDFType::BxDF_ALL, &bxdfType);
        NAN_DETECT_V(brdf, "WhiteFurnaceIntegrator::BSDF");
        INF_DETECT_V(brdf, "WhiteFurnaceIntegrator::BSDF");
        if (std::abs(pdf) == 0.f || brdf == glm::vec3(0))
        {
            return L;
        }

        bool specularPath = (bxdfType & BxDFType::BxDF_SPECULAR);
        bool transmission = (bxdfType & BxDFType::BxDF_TRANSMISSION);
        auto cosine = std::max(0.f, transmission ? glm::dot(-N, wIn) : glm::dot(N, wIn));

        Ray3f nextRay = isec.SpawnRay(wIn).Ray;

        Spectrum beta = Spectrum(1.f);

        if (specularPath)
        {
            L += brdf * scene->GetEnvironmentLight(nextRay.Dir());
        }
		return L;
	}

    Spectrum WhiteFurnaceIntegrator::UniformSampleAllLights(const SurfaceInteraction& isec, const RayScene* scene,
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

    Spectrum WhiteFurnaceIntegrator::EsimateDirect(const SurfaceInteraction& isec, const RayScene* scene,
        const Vector2f& sampleLight, const Vector2f& sampleBSDF,
        const crystal::Light* light, Sampler* sampler)
    {
        Spectrum L(0.f);

        BxDFType sampleType = (BxDFType)(BxDFType::BxDF_ALL & ~BxDFType::BxDF_SPECULAR);
        Point3f P = isec.GetHitPos();
        Normal3f N = isec.GetInteractionNormal();
        Vector3f wo = -isec.GetHitDir();

        auto bsdf = isec.GetBSDF();

        // Sample light source with MIS (Specular BSDF will not have value)
        {
            Point3f lightPos;
            float pdf_light;
            auto Li_light = light->Sample_Li(isec.GetGeometryInfo(false), sampleLight, &lightPos, &pdf_light);

            Vector3f wi = glm::normalize(lightPos - P);
            float NdotL = std::max(0.f, glm::dot(N, wi));
            if (pdf_light != 0.f && !std::isinf(pdf_light) && Li_light != Spectrum(0.f) && NdotL != 0.f)
            {
                Spectrum f = bsdf->DistributionFunction(wo, wi);
                float pdf_bsdf = bsdf->Pdf(wo, wi, sampleType);
                if (f != Spectrum(0.f))
                {
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
            Vector3f wi;
            Spectrum f = bsdf->SampleDirection(sampler->Get1D(), sampleBSDF, wo, &wi,
                &pdf_bsdf, sampleType, &sampledType);
            float NdotL = std::max(0.f, glm::dot(N, wi));

            if (f == Spectrum(0.f) || pdf_bsdf == 0.f || NdotL == 0.f)
            {
                return L;
            }
            bool specularBSDF = sampledType & BxDFType::BxDF_SPECULAR;

            float weight = 1.0f;
            if (!specularBSDF)
            {
                f *= NdotL;
                float pdf_light = light->Pdf_Li(isec.GetGeometryInfo(false), wi);
                if (pdf_light == 0.f || f == Spectrum(0.f))
                {
                    return L;
                }
                weight = PowerHeuristic(1, pdf_bsdf, 1, pdf_light);
            }

            Spectrum Li(0.f);
            Ray3f lightTestRay = isec.SpawnRay(wi).Ray;
            SurfaceInteraction lightIsec;

            if (light->GetFlags() & LightFlags::Infinite)
            {
                Li = light->Le(wi);
            }

            if (Li != Spectrum(0.f))
            {
                L += f * Li * weight / pdf_bsdf;
            }

            NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
            INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        }
        return L;
    }
}