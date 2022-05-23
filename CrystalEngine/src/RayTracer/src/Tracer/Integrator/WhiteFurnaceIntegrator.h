#pragma once
#include <memory>
#include "SamplerIntegrator.h"
#include <Function/Framework/Components/Light/Lights/Light.h>

namespace tracer
{
    class WhiteFurnaceIntegrator : public SamplerIntegrator
    {
    public:
        WhiteFurnaceIntegrator(const std::shared_ptr<Sampler>& sampler, int threads);

        void Preprocess(const RayScene* scene) override {};
        Spectrum Evaluate(const RayTr& ray, const RayScene* scene,
            Sampler* sampler) override;

    private:

        Spectrum eval_rec(const RayTr& ray, const RayScene* scene, Sampler* sampler, int level, bool specular);
        Spectrum EsimateDirect_NoTest(const SurfaceInteraction& isec, const RayScene* scene,
            const Vector2f& sampleLight, const Vector2f& sampleBSDF,
            const crystal::Light* light, Sampler* sampler);
    };
}