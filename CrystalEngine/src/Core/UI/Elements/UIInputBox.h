#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"
#include "UILabel.h"

namespace crystal
{
    class UIInputBox : public UIElement
    {
    public:
        UIInputBox();
        ~UIInputBox() override;

        virtual void OnFocused(UIEventArgs args) override;
        virtual void OnUnFocused(UIEventArgs args) override;

    protected:
        Color4f     m_backgroundColor{};
        Color4f     m_borderColor{};
        Color4f     m_focusBorderColor{};

        std::string             m_text{};
        std::shared_ptr<Font>   m_pFont = nullptr;

        bool        m_isFocused = false;
        double      m_blinkTimer = 0.0;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}