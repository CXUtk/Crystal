#pragma once
#include <Application.h>
#include <Engine.h>
#include <Function/UI/UIExports.h>

#include "Tracer/RayTracer.h"

namespace tracer
{
    class Presenter : public crystal::Application
    {
    public:
        Presenter();
        virtual ~Presenter() override;

        virtual void Initialize() override;

        virtual void Update(const crystal::GameTimer& gameTimer) override;

        virtual void Draw(const crystal::GameTimer& gameTimer) override;

        virtual void Exit() override;

        virtual bool Paused() override;

    private:
        bool                            m_renderPause = false;
        std::shared_ptr<RayTracer>      m_tracer = nullptr;
        std::shared_ptr<ITexture2D>     m_rayTraceTexture = nullptr;
        double                          m_elapsedTime = 0.f;
    };
}
