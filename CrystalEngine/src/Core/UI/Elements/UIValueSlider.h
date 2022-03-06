#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"
#include "UIPanel.h"
#include "Interfaces/IUIValueElement.h"

namespace crystal
{
    class UIValueSlider : public IUIValueElement<double>
    {
    public:
        UIValueSlider();
        ~UIValueSlider() override;

        Color4f GetBarColor() const { return m_barColor; }
        void SetBarColor(const Color4f& color) { m_barColor = color; }

        Color4f GetSliderColor() const { return m_sliderColor; }
        void SetSliderColor(const Color4f& color) { m_sliderColor = color; }

        virtual void MouseJustPressed(UIMouseButtonEventArgs args) override;
        virtual void MouseJustReleased(UIMouseButtonEventArgs args) override;

    protected:
        Color4f         m_barColor{};
        Color4f         m_sliderHoverColor{};
        Color4f         m_sliderColor{};

        bool            m_isDragging = false;
        Bound2f         m_barBound{};

        std::shared_ptr<UIPanel>        m_bar = nullptr;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
        virtual void UpdateSelf(const GameTimer& gameTimer) override;
        virtual void RecalculateChildren() override;
    };
}