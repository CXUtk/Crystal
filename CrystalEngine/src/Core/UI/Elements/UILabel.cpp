#include "UILabel.h"
#include <codecvt>

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    UILabel::UILabel(const std::string& text)
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();

        m_textDrawComponent = std::make_shared<UITextComponent>(assetManager->LoadAsset<Font>("Crystal:Consolas"),
            false);

        m_textDrawComponent->SetTextColor(Color4f(1.f));
        SetText(text);
    }

    std::string UILabel::GetText() const
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        return conv.to_bytes(m_text);
    }

    void UILabel::SetText(const std::string& text)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        auto text32 = conv.from_bytes(text);
        if (m_text != text32)
        {
            m_shouldRecalculateText = true;
        }
        m_text = text32;
        m_textDrawComponent->SetText32(m_text);
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

        m_shouldRecalculateText = false;
    }
}