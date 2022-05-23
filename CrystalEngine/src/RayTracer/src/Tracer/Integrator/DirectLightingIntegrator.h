#pragma once
#include <memory>
#include "SamplerIntegrator.h"
#include <Function/Framework/Components/Light/Lights/Light.h>

namespace tracer
{
    class DirectLightingIntegrator : public SamplerIntegrator
    {
    public:
        DirectLightingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth);

        void Preprocess(const RayScene* scene) override {};
        Spectrum Evaluate(const RayTr& ray, const RayScene* scene,
            Sampler* sampler) override;

    private:
        int m_maxDepth{};

        Spectrum eval_rec(const RayTr& ray, const RayScene* scene, Sampler* sampler, int level, bool specular);
    };
}