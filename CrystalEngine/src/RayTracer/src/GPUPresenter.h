#pragma once
#include <Application.h>
#include <Engine.h>
#include <Function/UI/UIExports.h>

#include "Tracer/RayTracer.h"

namespace tracer
{
    class GPUPresenter : public crystal::Application
    {
    public:
        GPUPresenter();
        virtual ~GPUPresenter() override;

        virtual void Initialize() override;

        virtual void Update(const crystal::GameTimer& gameTimer) override;

        virtual void Draw(const crystal::GameTimer& gameTimer) override;

        virtual void Exit() override;

        virtual bool Paused() override;

    private:
        std::shared_ptr<IRenderTarget2D>            m_screenFBuffer[2]{};

        std::shared_ptr<ITexture2D>                 m_sceneBuffer = nullptr;
        std::shared_ptr<ITexture2D>                 m_geometryDataBuffer = nullptr;

        std::shared_ptr<IPipelineStateObject>       m_PSO = nullptr;
        std::shared_ptr<IPipelineResourceObject>	m_PRO = nullptr;

        std::shared_ptr<IPipelineResourceObject>    m_PROScreen = nullptr;

        std::shared_ptr<IShaderProgram>             m_pShader = nullptr;
        std::shared_ptr<IShaderProgram>             m_pToneMapping = nullptr;
        std::shared_ptr<Camera>						m_pCamera = nullptr;
        std::shared_ptr<Transform>				    m_cameraTransform = nullptr;

        GPUDataPackage                              m_GPUData{};

        bool                            m_renderPause = false;
        double                          m_elapsedTime = 0.f;
        int                             m_iterations = 0;
    };
}
