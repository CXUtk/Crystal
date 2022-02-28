#pragma once
#include <Core/Application.h>
#include "Engine.h"
#include <Core/UI/UICommon.h>

namespace crystal
{
    class UITest : public Application
    {
    public:
        UITest();
        virtual ~UITest() override;

        virtual void Initialize() override;

        virtual void Update(const crystal::GameTimer& gameTimer) override;

        virtual void Draw(const crystal::GameTimer& gameTimer) override;

        virtual void Exit() override;

        virtual bool Paused() override;

    private:
        bool    m_renderPause = false;

        std::unique_ptr<UIStateMachine>     m_pUIStateMachine{};
    };
}
