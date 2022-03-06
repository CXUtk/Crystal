#include "UIPanel.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>

namespace crystal
{
    UIPanel::UIPanel() : UIElement()
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();
        m_drawColor = UIStyle::GetPanelInnerColor();
        m_borderColor = UIStyle::GetPanelBorderColor();
    }

    UIPanel::~UIPanel()
    {}

    void UIPanel::DrawSelf(const RenderPayload & payload, const GameTimer & gameTimer)
    {
        auto device = Engine::GetInstance()->GetGraphicsDevice();
        auto geometryRenderer = payload.GeometryRenderer;
        geometryRenderer->Begin(payload.PSO);
        geometryRenderer->DrawBound2DFill(BoundingBoxConvert<int>(m_calculatedInnerBound),
            m_drawColor, m_borderColor);
        geometryRenderer->End();
    }
}
