#pragma once
#include <memory>
#include "SamplerIntegrator.h"

namespace tracer
{
    class PathTracingIntegrator : public SamplerIntegrator
    {
    public:
        PathTracingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth);

        void Preprocess(const RayScene* scene) override {};
        Spectrum Evaluate(const Ray3f& ray, const RayScene* scene,
            Sampler* sampler) override;

    private:
        // glm::vec3 eval_rec(const Ray3f& ray, Scene* scene, Sampler* sampler, int level, bool specular);

        int m_maxDepth;

        Spectrum UniformSampleAllLights(const SurfaceInteraction& isec, const RayScene* scene, Sampler* sampler);
        Spectrum UniformSampleOneLight(const SurfaceInteraction& isec, const RayScene* scene, Sampler* sampler);
        Spectrum EsimateDirect(const SurfaceInteraction& isec, const RayScene* scene,
            const Vector2f& sampleLight, const Vector2f& sampleBSDF,
            const crystal::Light* light, Sampler* sampler);
    };
}