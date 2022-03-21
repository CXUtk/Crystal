#include "UIPanel.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/UI/UIStateMachine.h>

namespace crystal
{
    UIPanel::UIPanel() : UIElement()
    {
        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();
        m_whiteTexture = stateMachine->GetWhiteTexture();
        m_frameTexture = stateMachine->GetFrameTexture();

        m_drawColor = UIStyle::GetPanelInnerColor();
        m_borderColor = UIStyle::GetPanelBorderColor();
    }

    UIPanel::~UIPanel()
    {}

    void UIPanel::DrawSelf(const RenderPayload& payload, const GameTimer & gameTimer)
    {
        auto spriteBatch = payload.SpriteBatch;
        SliceInfo slice = {};
        slice.Left = 1;
        slice.Right = 1;
        slice.Top = 1;
        slice.Bot = 1;
        slice.DrawFlags = Slice_Nine;

        auto bound = BoundingBoxConvert<int>(GetInnerBound());
        if (m_drawColor != Color4f(0.f))
        {
            spriteBatch->Draw(m_whiteTexture, bound, m_drawColor);
        }
        spriteBatch->DrawSlicedTexture(m_frameTexture, slice, bound, m_borderColor);
    }
}
