#include "Canvas.h"

#include <Engine.h>

#include <Resource/Asset/AssetManager.h>

#include <Function/Render/RenderExports.h>
#include <Function/Input/InputController.h>
#include <Function/UI/UIStateMachine.h>

using namespace crystal;

namespace tracer
{
    Canvas::Canvas()
    {}

    Canvas::~Canvas()
    {}
    void Canvas::DrawSelf(const crystal::RenderPayload & payload, const crystal::GameTimer & gameTimer)
    {
        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();
        auto spriteBatch = payload.SpriteBatch;
        spriteBatch->Draw(stateMachine->GetWhiteTexture(), BoundingBoxConvert<int>(GetInnerBound()),
                Color4f(0.f, 0.f, 0.f, 1.f));
    }
}
