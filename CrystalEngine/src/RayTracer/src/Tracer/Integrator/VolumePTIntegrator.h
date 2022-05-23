#pragma once
#include <memory>
#include "SamplerIntegrator.h"

namespace tracer
{
    class VolumePTIntegrator : public SamplerIntegrator
    {
    public:
        VolumePTIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth);

        void Preprocess(const RayScene* scene) override {};
        Spectrum Evaluate(const RayTr& ray, const RayScene* scene,
            Sampler* sampler) override;

    private:
        // glm::vec3 eval_rec(const Ray3f& ray, Scene* scene, Sampler* sampler, int level, bool specular);

        int m_maxDepth;
        //Spectrum TraceLiM(const RayScene* scene, const MediumInteractionInfo& mi, const Point3f& lightPos);
    };
}