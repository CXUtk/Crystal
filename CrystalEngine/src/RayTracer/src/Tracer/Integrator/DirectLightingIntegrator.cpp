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

    Spectrum tracer::DirectLightingIntegrator::Evaluate(const RayTr& ray, const RayScene* scene, Sampler* sampler)
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
        L += UniformSampleAllLights(isec, scene, sampler, false);
        return L;
    }
}