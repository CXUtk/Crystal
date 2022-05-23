#include "DirectLightingIntegrator.h"
#include <glm/gtx/transform.hpp>
#include <Function/Framework/Components/Material/BSDF.h>

#include <Core/Sampling/Sampling.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>

namespace tracer
{
    static constexpr float EPS = 1e-4;

	DirectLightingIntegrator::DirectLightingIntegrator(const std::shared_ptr<Sampler>& sampler,
		int threads, int maxDepth) : SamplerIntegrator(sampler, threads), m_maxDepth(maxDepth)
	{}

    Spectrum tracer::DirectLightingIntegrator::Evaluate(const RayTr& ray, const RayScene * scene, Sampler * sampler)
    {
        return eval_rec(ray, scene, sampler, 0, true);
    }

	Spectrum DirectLightingIntegrator::eval_rec(const RayTr& ray, const RayScene* scene, Sampler* sampler,
		int level, bool specular)
	{
		Spectrum L(0.f);
		if (level == m_maxDepth) return L;

		SurfaceInteraction isec;
		if (!scene->Intersect(ray.Ray, &isec))
		{
            return scene->GetEnvironmentLight(ray.Ray.Dir());
		}

		// Get emitted radiance from hit surface to such direction
		L += isec.Le(-ray.Ray.Dir());

		isec.SetBSDF(std::make_shared<BSDF>(&isec));
		isec.GetMaterial()->ComputeScatteringFunctions(&isec, true);

		// No bsdf function, means the object is transparent
		if (!isec.GetBSDF() || isec.GetBSDF()->IsEmpty())
		{
			return eval_rec(isec.SpawnRay(ray.Ray.Dir()), scene, sampler, level, specular);
		}
		L += UniformSampleAllLights(isec, scene, sampler);
		return L;
	}

    Spectrum DirectLightingIntegrator::UniformSampleAllLights(const SurfaceInteraction& isec, const RayScene* scene,
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

    Spectrum DirectLightingIntegrator::EsimateDirect(const SurfaceInteraction& isec, const RayScene* scene,
        const Vector2f& sampleLight, const Vector2f& sampleBSDF,
        const crystal::Light* light, Sampler* sampler)
    {
        Spectrum L(0.f);

        BxDFType sampleType = (BxDFType)(BxDFType::BxDF_ALL & ~BxDFType::BxDF_SPECULAR);
        Point3f P = isec.GetPosition();
        Normal3f N = isec.GetInteractionNormal();
        Vector3f wo = -isec.GetW_Out();

        auto bsdf = isec.GetBSDF();


        // Sample light source with MIS (Specular BSDF will not have value)
        Point3f lightPos;
        float pdf_light;
        auto Li_light = light->Sample_Li(isec.GetGeometryInfo(), sampleLight, &lightPos, &pdf_light);

        Vector3f wi = glm::normalize(lightPos - P);
        float NdotL = std::max(0.f, glm::dot(N, wi));
        if (pdf_light != 0.f && !std::isinf(pdf_light) && Li_light != Spectrum(0.f) && NdotL != 0.f)
        {
            Spectrum f = bsdf->DistributionFunction(wo, wi);
            float pdf_bsdf = bsdf->Pdf(wo, wi, sampleType);
            if (f != Spectrum(0.f))
            {
                if (scene->IntersectTest(isec.SpawnRayTo(lightPos).Ray, 0, 1.f - EPS))
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

        // Sample BSDF (Delta light should not have any value in BSDF sample)
        if (!light->IsDeltaLight())
        {
            float pdf_bsdf = 0.f;
            BxDFType sampledType;
            Spectrum f = bsdf->SampleDirection(sampler->Get1D(), sampleBSDF, wo, &wi,
                &pdf_bsdf, sampleType, &sampledType);

            if (f == Spectrum(0.f) || pdf_bsdf == 0.f || NdotL != 0.f)
            {
                return L;
            }
            bool specularBSDF = sampledType & BxDFType::BxDF_SPECULAR;

            float weight = 1.0f;
            if (!specularBSDF)
            {
                f *= std::max(0.f, glm::dot(N, wi));
                pdf_light = light->Pdf_Li(isec.GetGeometryInfo(), wi);
                if (pdf_light == 0.f || f == Spectrum(0.f))
                {
                    return L;
                }
                weight = PowerHeuristic(1, pdf_bsdf, 1, pdf_light);
            }


            Spectrum Li(0.f);
            Ray3f lightTestRay = isec.SpawnRay(wi).Ray;
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
                L += f * Li * weight / pdf_bsdf;
            }
        }

        NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
        return L;
    }
}