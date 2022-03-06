#include "UIProgressBar.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

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
        auto device = Engine::GetInstance()->GetGraphicsDevice();
        auto geometryRenderer = payload.GeometryRenderer;
        geometryRenderer->Begin(payload.PSO);
        auto outerBound = BoundingBoxConvert<int>(m_calculatedInnerBound);
        geometryRenderer->DrawBound2DFill(outerBound,
            m_backgroundColor, m_borderColor);

        auto minPos = outerBound.GetMinPos();
        auto maxPos = outerBound.GetMaxPos();
        auto width = (int)((maxPos.x - minPos.x - 2) * m_value);
        auto progressBound = Bound2i(minPos + Vector2i(1, 1), Vector2i(minPos.x + 1 + width, maxPos.y - 1));
        geometryRenderer->DrawBound2DFill(progressBound,
            m_barColor);
        geometryRenderer->End();
    }
}
