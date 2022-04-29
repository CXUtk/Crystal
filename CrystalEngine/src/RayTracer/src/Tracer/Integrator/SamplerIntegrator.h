#pragma once
#include <memory>
#include "Integrator.h"
#include <Tracer/Sampler/Sampler.h>
#include <Core/Utils/FixedThreadPool.h>

namespace tracer
{
    class SamplerIntegrator : public Integrator
    {
    public:
        SamplerIntegrator(const std::shared_ptr<Sampler>& sampler, int threads);

        virtual void Preprocess(const RayScene* scene) = 0;
        void Render(const RayScene* rayScene,
            const CameraComponent* camera, FrameBuffer* frameBuffer) override;
        virtual Spectrum Evaluate(const Ray3f& ray, const RayScene* scene,
            Sampler* sampler) = 0;

    private:
        std::shared_ptr<Sampler> _sampler;
        std::shared_ptr<FixedThreadPool> _threadPool;
        int _numThreads;
    };
}