#pragma once
#include "../../Common/UICommon.h"
#include "../../Common/UIEventArgs.h"

#include "../UIElement.h"
#include "../UILabel.h"
#include "../Components/UITextComponent.h"
#include "../Components/UIInputComponent.h"

namespace crystal
{
    class UIInputBox : public UIElement
    {
    public:
        UIInputBox();
        ~UIInputBox() override;

        std::shared_ptr<Font> GetFont() const { return m_textDrawComponent->GetFont(); }
        void SetFont(std::shared_ptr<Font> font)
        {
            if (m_textDrawComponent->GetFont() != font)
            {
                m_isStateDirty = true;
            }
            m_textDrawComponent->SetFont(font);
        }

        std::string GetText() const;
        void SetText(const std::string& text);

        virtual void OnFocused(UIEventArgs args) override;
        virtual void OnUnFocused(UIEventArgs args) override;
        virtual void MouseJustPressed(UIMouseButtonEventArgs args) override;
        virtual void MouseJustReleased(UIMouseButtonEventArgs args) override;

    protected:
        Color4f     m_backgroundColor{};
        Color4f     m_borderColor{};
        Color4f     m_focusBorderColor{};

        std::shared_ptr<UITextComponent>    m_textDrawComponent = nullptr;
        std::shared_ptr<UIInputComponent>   m_inputComponent = nullptr;

        bool        m_isFocused = false;
        double      m_blinkTimer = 0.0;
        float       m_drawXOffset = 0.f;
        int         m_dragStartIndex = 0;
        int         m_dragEndIndex = 0;
        bool        m_isDragging = false;

        std::vector<float>      m_charWidths{};

        UIActionEvent       m_eventOnEnterPressed{};

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
        virtual void UpdateSelf(const GameTimer& gameTimer) override;
        int GetCarrotByPos(const Vector2i& screenPos);
        /**
         * @brief Get the exact width from the beginning of the string to the carrot postition
         * @param carrot 
         * @return 
        */
        float GetXOffsetByCarrot(int carrot);
        void UpdateCarrotShift();
        void RespondToChange();
    };
}