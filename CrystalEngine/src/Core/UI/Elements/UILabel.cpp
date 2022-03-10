#include "UILabel.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    UILabel::UILabel()
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();
        m_pFont = assetManager->LoadAsset<Font>("Crystal:Consolas");

        m_textColor = Color4f(1.f);
    }

    UILabel::~UILabel()
    {}

    void UILabel::UpdateSelf(const GameTimer& gameTimer)
    {
        if (m_shouldRecalculateText)
        {
            RecalculateSelf();
        }
    }

    void UILabel::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto spriteBatch = payload.SpriteBatch;
        spriteBatch->DrawString(m_pFont, m_text, m_calculatedInnerBound.GetMinPos() + m_originOffset,
            Color4f(1.f));
    }

    void UILabel::RecalculateSelf()
    {
        auto metric = m_pFont->MeasureString(m_text);
        auto height = metric.yMax - metric.yMin;
        SetSize(SizeLayout(metric.Width, 0, height, 0));
        m_originOffset = Vector2f(0.f, -metric.yMin);

        m_shouldRecalculateText = false;
    }
}