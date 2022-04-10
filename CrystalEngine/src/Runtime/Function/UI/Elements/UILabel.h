#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"
#include "Components/UITextComponent.h"
#include "Resource/Asset/Content/Font.h"

namespace crystal
{
    class UILabel : public UIElement
    {
    public:
        UILabel(const std::string& text);
        ~UILabel() override;

        Color4f GetTextColor() const { return m_textDrawComponent->GetTextColor(); }
        void SetTextColor(const Color4f& color) { m_textDrawComponent->SetBorderColor(color); }

        Color4f GetBorderColor() const { return m_textDrawComponent->GetBorderColor(); }
        void SetBorderColor(const Color4f& color) { m_textDrawComponent->SetBorderColor(color); }

        std::string GetText() const;
        void SetText(const std::string& text);

        std::shared_ptr<Font> GetFont() const { return m_textDrawComponent->GetFont(); }
        void SetFont(std::shared_ptr<Font> font)
        {
            if (m_textDrawComponent->GetFont() != font)
            {
                m_shouldRecalculateText = true;
            }
            m_textDrawComponent->SetFont(font);
        }

        virtual void UpdateSelf(const GameTimer& gameTimer) override;

        Vector2f GetPredictedSize() const;

        virtual void RecalculateWidth() override;
        virtual void RecalculateHeight() override;

    protected:
        bool                m_shouldRecalculateText = true;
        std::u32string      m_text{};

        std::shared_ptr<UITextComponent>   m_textDrawComponent = nullptr;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
        virtual void RecalculateSelf() override;
    };
}