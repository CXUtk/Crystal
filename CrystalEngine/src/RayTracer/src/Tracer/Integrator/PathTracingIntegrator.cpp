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


    Spectrum PathTracingIntegrator::Evaluate(const RayTr& ray, const RayScene* scene,
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

            Vector3f wo = -currentRay.Ray.Dir();
            Normal3f N = isec.GetInteractionNormal();
            Point3f P = isec.GetPosition();

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

            if (!isec.GetMaterial())
            {
                currentRay = isec.SpawnRay(currentRay.Ray.Dir());
                continue;
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
            L += beta * UniformSampleAllLights(isec, scene, sampler, false);
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
}