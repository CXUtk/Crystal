#pragma once
#include <Application.h>
#include <Engine.h>
#include <Function/UI/UIExports.h>

namespace tracer
{
    class CrystalTracer : public crystal::Application
    {
    public:
        CrystalTracer();
        virtual ~CrystalTracer() override;

        virtual void Initialize() override;

        virtual void Update(const crystal::GameTimer& gameTimer) override;

        virtual void Draw(const crystal::GameTimer& gameTimer) override;

        virtual void Exit() override;

        virtual bool Paused() override;

    private:
        bool    m_renderPause = false;
    };
}
