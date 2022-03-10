#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "Interfaces/IUIValueElement.h"
#include "UIPanel.h"

namespace crystal
{
    class UIScrollBarBase : public IUIValueElement<double>
    {
    public:
        UIScrollBarBase();

        Color4f GetBarColor() const { return m_barColor; }
        void SetBarColor(const Color4f& color) { m_barColor = color; }

        Color4f GetSliderColor() const { return m_sliderColor; }
        void SetSliderColor(const Color4f& color) { m_sliderColor = color; }

        /**
         * @brief Get the view range / size of value bar
         * @return
        */
        double GetViewRange() const { return m_viewRange; }
        void SetViewRange(double range) { m_viewRange = range; }

        virtual void MouseJustPressed(UIMouseButtonEventArgs args) override;
        virtual void MouseJustReleased(UIMouseButtonEventArgs args) override;

    protected:
        Color4f         m_barColor{};
        Color4f         m_sliderHoverColor{};
        Color4f         m_sliderColor{};

        double          m_viewRange = 0.f;
        bool            m_isDragging = false;
        bool            m_isLocating = false;
        Vector2i        m_posDragStartOffset{};

        std::shared_ptr<UIPanel>        m_bar = nullptr;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };

    class UIScrollBarV : public UIScrollBarBase
    {
    public:
        UIScrollBarV();
        ~UIScrollBarV() override;

    protected:
        virtual void UpdateSelf(const GameTimer& gameTimer) override;
    };

    class UIScrollBarH : public UIScrollBarBase
    {
    public:
        UIScrollBarH();
        ~UIScrollBarH() override;

    protected:
        virtual void UpdateSelf(const GameTimer& gameTimer) override;
    };
}

   