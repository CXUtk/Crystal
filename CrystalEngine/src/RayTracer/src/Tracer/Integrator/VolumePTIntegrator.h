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

        Spectrum UniformSampleAllLights(const SurfaceInteraction& isec, const RayScene* scene, Sampler* sampler, bool handleMedium);
        Spectrum UniformSampleOneLight(const SurfaceInteraction& isec, const RayScene* scene, Sampler* sampler, bool handleMedium);
        Spectrum UniformSampleOneLightMedium(const MediumInteractionInfo& mi, const RayScene* scene, Sampler* sampler);
        Spectrum EsimateDirect(const SurfaceInteraction& isec, const RayScene* scene,
            const Vector2f& sampleLight, const Vector2f& sampleBSDF,
            const crystal::Light* light, Sampler* sampler, bool handleMedium);

        Spectrum EsimateDirectMedium(const MediumInteractionInfo& mi, const RayScene* scene,
            const Vector2f& sampleLight, const Vector2f& sampleBSDF,
            const crystal::Light* light, Sampler* sampler, bool handleMedium);

        Spectrum TraceLi(const RayScene* scene, const SurfaceInteraction& isec, const Point3f& lightPos);
        Spectrum TraceLiM(const RayScene* scene, const MediumInteractionInfo& mi, const Point3f& lightPos);
    };
}