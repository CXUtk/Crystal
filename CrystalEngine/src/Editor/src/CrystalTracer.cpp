#include "CrystalTracer.h"

#include <Core/Utils/Logger.h>
#include <Core/Utils/Geometry.h>
#include <Core/Utils/GameTimer.h>

#include <Function/Input/InputController.h>
#include <Function/Render/SpriteBatch.h>
#include <Function/UI/UIStateMachine.h>

#include "UI/EditorState.h"

using namespace crystal;

namespace tracer
{
    CrystalTracer::CrystalTracer()
        : crystal::Application()
    {}

    CrystalTracer::~CrystalTracer()
    {}

    void CrystalTracer::Initialize()
    {
        auto UISM = Engine::GetInstance()->GetUIStateMachine();

        UISM->AddState("editor", std::make_shared<EditorState>());
        UISM->SetState("editor");
    }

    void CrystalTracer::Update(const crystal::GameTimer& gameTimer)
    {
        auto UISM = Engine::GetInstance()->GetUIStateMachine();
        UISM->Update(gameTimer);
    }

    void CrystalTracer::Draw(const crystal::GameTimer& gameTimer)
    {
        auto graphicsContext = m_engine->GetGraphicsContext();
        graphicsContext->Clear(
            crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
            | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
            | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
            crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);

        auto UISM = Engine::GetInstance()->GetUIStateMachine();
        UISM->Draw(gameTimer);
    }

    void CrystalTracer::Exit()
    {

    }

    bool CrystalTracer::Paused()
    {
        return false;
    }
}