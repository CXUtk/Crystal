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

        m_textDrawComponent = std::make_shared<UITextComponent>(assetManager->LoadAsset<Font>("Crystal:Consolas"),
            false);

        m_textDrawComponent->SetTextColor(Color4f(1.f));
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
        m_textDrawComponent->DrawWithBot(m_calculatedInnerBound.GetMinPos(), payload);
    }

    void UILabel::RecalculateSelf()
    {
        auto metric = m_textDrawComponent->MeasureSize();
        auto height = metric.yMax - metric.yMin;
        SetSize(SizeLayout(metric.Width, 0, height, 0));
        m_originOffset = Vector2f(0.f, -metric.yMin);

        m_shouldRecalculateText = false;
    }
}