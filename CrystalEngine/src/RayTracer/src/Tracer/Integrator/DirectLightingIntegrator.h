#pragma once
#include <memory>
#include "SamplerIntegrator.h"
#include <Function/Framework/Components/Light/Light.h>

namespace tracer
{
    class DirectLightingIntegrator : public SamplerIntegrator
    {
    public:
        DirectLightingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth);

        void Preprocess(const RayScene* scene) override {};
        Spectrum Evaluate(const Ray3f& ray, const RayScene* scene,
            Sampler* sampler) override;

    private:
        int m_maxDepth{};

        Spectrum eval_rec(const Ray3f& ray, const RayScene* scene, Sampler* sampler, int level, bool specular);
        Spectrum UniformSampleAllLights(const SurfaceInteraction& isec, const RayScene* scene, Sampler* sampler);
        Spectrum EsimateDirect(const SurfaceInteraction& isec, const RayScene* scene,
            const Vector2f& sampleLight, const Vector2f& sampleBSDF,
            const crystal::LightComponent* light, Sampler* sampler);
    };
}