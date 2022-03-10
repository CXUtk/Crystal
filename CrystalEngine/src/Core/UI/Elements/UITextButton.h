#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"
#include "UILabel.h"
#include "UIPanel.h"

namespace crystal
{
    class UITextButton : public UIElement
    {
    public:
        UITextButton();
        ~UITextButton() override;

        Color4f GetMouseHoverColor() const { return m_hoverColor; }
        void SetMouseHoverColor(const Color4f& color) { m_hoverColor = color; }

        Color4f GetNormalColor() const { return m_unhoverColor; }
        void SetNormalColor(const Color4f& color) { m_unhoverColor = color; }

        std::string GetText() const { return m_buttonText->GetText(); }
        void SetText(const std::string& text) { m_buttonText->SetText(text); }

        void MouseEnter(UIMouseEventArgs args) override;
        void MouseLeave(UIMouseEventArgs args) override;

    protected:
        Color4f     m_hoverColor{};
        Color4f     m_unhoverColor{};

        std::shared_ptr<UIPanel>     m_penel = nullptr;
        std::shared_ptr<UILabel>     m_buttonText = nullptr;

        bool    m_isMouseHover = false;


        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}