#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"

namespace crystal
{
    class UIProgressBar : public UIElement
    {
    public:
        UIProgressBar();
        ~UIProgressBar() override;

        Color4f GetBackgroundColor() const { return m_backgroundColor; }
        void SetBackgroundColor(const Color4f& color) { m_backgroundColor = color; }

        Color4f GetBorderColor() const { return m_borderColor; }
        void SetBorderColor(const Color4f& color) { m_borderColor = color; }

        Color4f GetBarColor() const { return m_barColor; }
        void SetBarColor(const Color4f& color) { m_barColor = color; }

        float GetValue() const { return m_value; }
        void SetValue(float value) { m_value = value; }

    protected:
        Color4f     m_backgroundColor{};
        Color4f     m_borderColor{};
        Color4f     m_barColor{};

        float       m_value = 0.f;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}