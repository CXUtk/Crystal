#pragma once
#include <memory>
#include "Integrator.h"
#include <Core/Sampling/Sampler/Sampler.h>
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
        virtual bool IsFinished() const override { return m_completedBlocks == m_totalBlocks; }
        virtual Spectrum Evaluate(const RayTr& ray, const RayScene* scene,
            Sampler* sampler) = 0;

    protected:
        Spectrum UniformSampleAllLights(const InteractionInfo& it, const RayScene* scene, Sampler* sampler, bool handleMedium);
        Spectrum UniformSampleOneLight(const InteractionInfo& it, const RayScene* scene, Sampler* sampler, bool handleMedium);
        Spectrum EsimateDirect(const InteractionInfo& it, const RayScene* scene,
            const Vector2f& sampleLight, const Vector2f& sampleBSDF,
            const crystal::Light* light, Sampler* sampler, bool handleMedium);

        Spectrum TraceLi(const RayScene* scene, const InteractionInfo& it, const Point3f& lightPos);

    private:
        std::shared_ptr<Sampler> m_sampler = nullptr;
        std::shared_ptr<FixedThreadPool> m_threadPool = nullptr;

        volatile size_t         m_completedBlocks{};
        int                     m_numThreads{};
        size_t                  m_totalBlocks{};
        Point2i                 m_nTiles{};
        std::vector<bool>       m_visit{};
        double                  m_startRenderingTime{};
        std::mutex              m_countLock{};

        struct TileNode
        {
            int x, y;
        };
        struct IntegratorBlock
        {
            size_t                  TargetWidth, TargetHeight;
            size_t                  SampleNumBegin, SampleNumEnd;
            size_t                  TileXBegin, TileXEnd;
            size_t                  TileYBegin, TileYEnd;
            size_t                  X, Y;
            size_t                  Seed;

            const RayScene*         RayScene;
            const CameraComponent*  Camera;
            FrameBuffer*            FrameBuffer;
        };


        void PushNewBlock(IntegratorBlock block);
    };
}