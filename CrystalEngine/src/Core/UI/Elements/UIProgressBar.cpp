#include "UIProgressBar.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>
#include <Core/UI/UIStateMachine.h>

namespace crystal
{
    UIProgressBar::UIProgressBar()
    {
        m_backgroundColor = UIStyle::GetProgressBarBackgroundColor();
        m_borderColor = UIStyle::GetPanelBorderColor();
        m_barColor = UIStyle::GetPanelBorderColor();
    }

    UIProgressBar::~UIProgressBar()
    {
    }

    void UIProgressBar::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();
        auto outerBound = BoundingBoxConvert<int>(m_calculatedInnerBound);
        auto spriteBatch = payload.SpriteBatch;
        spriteBatch->Draw(stateMachine->GetWhiteTexture(), BoundingBoxConvert<int>(outerBound),
            m_backgroundColor);
        SliceInfo slice = {};
        slice.Left = 1;
        slice.Right = 1;
        slice.Top = 1;
        slice.Bot = 1;
        slice.DrawFlags = Slice_Nine;
        spriteBatch->DrawSlicedTexture(stateMachine->GetFrameTexture(), slice, BoundingBoxConvert<int>(outerBound),
            m_borderColor);

        auto minPos = outerBound.GetMinPos();
        auto maxPos = outerBound.GetMaxPos();
        auto width = (int)((maxPos.x - minPos.x - 2) * m_value);
        auto progressBound = Bound2i(minPos + Vector2i(1, 1), Vector2i(minPos.x + 1 + width, maxPos.y - 1));

        spriteBatch->Draw(stateMachine->GetWhiteTexture(), BoundingBoxConvert<int>(progressBound),
                m_barColor);
    }
}
