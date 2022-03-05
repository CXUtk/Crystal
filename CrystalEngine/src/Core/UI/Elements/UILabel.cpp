#include "UILabel.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

#include "UIIconButton.h"

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
            m_shouldRecalculateText = false;
        }
    }

    void UILabel::RecalculateSelf()
    {
        auto metric = m_pFont->MeasureString(m_text);
        auto height = metric.yMax - metric.yMin;
        m_size = SizeLayout(metric.Width, 0, height, 0);
        m_originOffset = Vector2f(0.f, -metric.yMin);

        UIElement::RecalculateSelf();
    }

    void UILabel::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto device = Engine::GetInstance()->GetGraphicsDevice();
        auto spriteBatch = payload.SpriteBatch;
        spriteBatch->Begin(SpriteSortMode::Deferred, nullptr,
            device->GetCommonBlendState(BlendStates::AlphaBlend));
        spriteBatch->DrawString(m_pFont, m_text, m_calculatedInnerBound.GetMinPos() + m_originOffset,
            Color4f(1.f));
        spriteBatch->End();
    }
}
