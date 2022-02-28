#include "UITest.h"
#include <Core/Utils/Logger.h>
#include <Core/Utils/Geometry.h>
#include <Core/Input/InputController.h>
#include <Core/Utils/GameTimer.h>
#include <Core/Render/SpriteBatch.h>

#include <Core/UI/UIStateMachine.h>
#include "UIStates/TestState.h"

namespace crystal
{
    UITest::UITest()
    {}

    UITest::~UITest()
    {}

    void UITest::Initialize()
    {
        m_pUIStateMachine = std::make_unique<UIStateMachine>();

        m_pUIStateMachine->AddState("test", std::make_shared<TestState>());
        m_pUIStateMachine->SetInitState("test");
    }

    void UITest::Update(const crystal::GameTimer& gameTimer)
    {
        m_pUIStateMachine->Update(gameTimer);
    }

    void UITest::Draw(const crystal::GameTimer& gameTimer)
    {
        auto graphicsContext = m_engine->GetGraphicsContext();
        graphicsContext->Clear(
            crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
            | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
            | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
            crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
        m_pUIStateMachine->Draw(gameTimer);
    }

    void UITest::Exit()
    {

    }

    bool UITest::Paused()
    {
        return false;
    }
}