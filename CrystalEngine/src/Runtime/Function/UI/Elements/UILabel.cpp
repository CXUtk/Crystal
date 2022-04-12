#include "UILabel.h"
#include <Engine.h>

#include "Resource/Asset/AssetManager.h"

#include "Function/Render/RenderExports.h"
#include "Function/Input/InputController.h"

#include "utf8.h"

namespace crystal
{
    UILabel::UILabel(const std::string& text)
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();

        auto font = assetManager->LoadAssetBuiltIn<FontFamily>("consola")->GetFont(18);
        m_textDrawComponent = std::make_shared<UITextComponent>(font, false);

        m_textDrawComponent->SetTextColor(Color4f(1.f));
        SetText(text);
    }

    std::string UILabel::GetText() const
    {
        return utf8::utf32to8(m_text);
    }

    void UILabel::SetText(const std::string& text)
    {
        auto text32 = utf8::utf8to32(text);
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

    Vector2f UILabel::GetPredictedSize() const
    {
        auto metric = m_textDrawComponent->MeasureSize();
        auto height = metric.yMax - metric.yMin;
        return Vector2f(metric.Width, height);
    }

    void UILabel::RecalculateWidth()
    {
        auto metric = m_textDrawComponent->MeasureSize();
        m_calculatedWidth = metric.Width;
    }

    void UILabel::RecalculateHeight()
    {
        auto metric = m_textDrawComponent->MeasureSize();
        m_calculatedHeight = metric.yMax - metric.yMin;
    }

    void UILabel::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        m_textDrawComponent->DrawWithBot(GetInnerBound().GetMinPos(), payload);
    }

    void UILabel::RecalculateSelf()
    {
        auto metric = m_textDrawComponent->MeasureSize();
        auto height = metric.yMax - metric.yMin;
        SetSize(SizeLayout(metric.Width, 0, height, 0));

        m_shouldRecalculateText = false;
    }
}