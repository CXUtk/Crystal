#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"
#include "../Common/UIStyle.h"

namespace crystal
{
    class UIElement : public std::enable_shared_from_this<UIElement>
    {
    public:
        UIElement();
        virtual ~UIElement();

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const RenderPayload& payload, const GameTimer& gameTimer);
        virtual void Recalculate();

        // Add Listeners
        template<UIEventType E>
        void AddEventListener(UIEventListener<E> listener);

        void AppendChild(std::shared_ptr<UIElement> element);
        void RemoveChild(std::shared_ptr<UIElement> element);
        void RemoveAllChildren();

        std::shared_ptr<UIElement> GetChildByName(const std::string& name);
        /**
         * @brief Get the element that supposed to respond to the event at given position,
         * if no such element exist, then [this] becomes that element
         * @param screenPos: Given position
         * @return
        */
        std::shared_ptr<UIElement> GetResponseElement(const Vector2f& screenPos);


        // Events Handler
        virtual void MouseJustPressed(UIMouseButtonEventArgs args);
        virtual void MouseJustReleased(UIMouseButtonEventArgs args);
        virtual void MouseClicked(UIMouseButtonEventArgs args);
        virtual void MouseDoubleClicked(UIMouseButtonEventArgs args);
        virtual void MouseEnter(UIMouseEventArgs args);
        virtual void MouseLeave(UIMouseEventArgs args);
        virtual void MouseScroll(UIMouseScrollEventArgs args);
        virtual void OnFocused(UIEventArgs args);
        virtual void OnUnFocused(UIEventArgs args);



        bool IsActive() const { return m_isActive; }
        void SetActive(bool value) { m_isActive = value; }

        bool IsEnabled() const { return m_isEnabled; }
        void SetEnabled(bool value) { m_isEnabled = value; }

        bool IsVisible() const { return m_isVisible; }
        void SetVisible(bool value) { m_isVisible = value; }

        bool NoEvents() const { return m_noEvents; }
        void SetNoEvents(bool value) { m_noEvents = value; }


        bool CanResponseEvent() const { return m_isActive && m_isEnabled && !m_noEvents; }

        std::string GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }

        std::string GetTooltip() const { return m_tooltip; }
        void SetTooltip(const std::string& tooltip) { m_tooltip = tooltip; }

        OverflowStyle GetOverflowStyle() const { return m_overflowStyle; }
        void SetOverflowStyle(OverflowStyle style) { m_overflowStyle = style; }

        PropagationFlags GetPropagationFlags() const { return m_propagationFlags; }
        void SetPropagationFlags(PropagationFlags flags) { m_propagationFlags = flags; }

        Vector2f GetPivot() const { return m_pivot; }
        void SetPivot(Vector2f pivot);

        Vector2f GetPosition() const { return m_position; }
        void SetPosition(Vector2f pos);

        Vector2f GetAnchorPoint() const { return m_anchorPoint; }
        void SetAnchorPoint(Vector2f anchorPoint);

        SizeLayout GetSize() const { return m_size; }
        void SetSize(SizeLayout size);

        int GetWidth() const;
        int GetHeight() const;

        Vector2i GetEstimatedSize(UIElement* fakeParent) const;

        Bound2f GetEventBound() const { return m_calculatedOuterBound; }
        Bound2f GetInnerBound() const { return m_calculatedInnerBound; }

        Vector2f GetPivotScreenPos() const;
        Bound2f GetParentBound() const;
        Vector2f GetLocalPositionToScreenPos(const Vector2f& localPos) const;
        Vector2f GetScreenPositionToLocalPos(const Vector2f& screenPos) const;

    protected:
        /// Data

        // Events
        UIEvent<UIEventType::OnMouseJustPressed>    m_eventOnMouseJustPress{};
        UIEvent<UIEventType::OnMouseJustReleased>   m_eventOnMouseJustRelease{};
        UIEvent<UIEventType::OnMouseClicked>        m_eventOnMouseClicked{};
        UIEvent<UIEventType::OnMouseDoubleClicked>  m_eventOnMouseDoubleClicked{};
        UIEvent<UIEventType::OnMouseEnter>          m_eventOnMouseEnter{};
        UIEvent<UIEventType::OnMouseLeave>          m_eventOnMouseLeave{};
        UIEvent<UIEventType::OnMouseScroll>         m_eventOnMouseScroll{};
        UIEvent<UIEventType::PostUpdate>            m_eventPostUpdate{};
        UIEvent<UIEventType::PostDraw>              m_eventPostDraw{};
        UIEvent<UIEventType::OnFocused>             m_eventOnFocused{};
        UIEvent<UIEventType::OnUnFocused>           m_eventOnUnFocused{};

        // Relationship

        /**
         * @brief Children are drawn from front to back, that means the first element will be at the bottom
        */
        std::vector<std::shared_ptr<UIElement>>     m_pChildren{};
        UIElement*                                  m_pParent = nullptr;


        // Properties
        bool        m_isActive = true;
        bool        m_isEnabled = true;
        bool        m_isVisible = true;
        bool        m_noEvents = false;
        bool        m_isFocused = false;
        bool        m_isStateDirty = true;

        std::string     m_name{};
        std::string     m_tooltip{};

        OverflowStyle       m_overflowStyle = OverflowStyle::Overflow;
        PropagationFlags    m_propagationFlags = PropagationFlags::All;

        /**
         * @brief Position is the displacement of pivot towards anchor point.
        */
        Vector2f        m_position{};
        /**
         * @brief Pivot is the position of current container with respect to
         * currrent container size.
        */
        Vector2f        m_pivot{};
        /**
         * @brief Anchor point is the relative position of pivot with
         * respect to the parent container.
        */
        Vector2f        m_anchorPoint{};
        /**
         * @brief Size has two components, one is the absolute size in pixels, another
         * is the relative size according to the size of parent container. Final
         * size is calculated by R * ParentSize + A.
        */
        SizeLayout      m_size{};

        // Not recommended
        Vector2f        m_scale{};
        Float           m_rotation{};

        // Auxiliary data
        Bound2f         m_calculatedInnerBound{};
        Bound2f         m_calculatedOuterBound{};


        // Virtual
        virtual void UpdateSelf(const GameTimer& gameTimer);
        virtual void UpdateChildren(const GameTimer& gameTimer);
        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer);
        virtual void DrawChildren(const RenderPayload& payload, const GameTimer& gameTimer);

        virtual void RecalculateSelf();
        virtual void RecalculateChildren();

        void CalculateBounds();
    };

    template<UIEventType E>
    inline void UIElement::AddEventListener(UIEventListener<E> listener)
    {
        static_assert(false, "Unknown UI Event Listener Type");
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseJustPressed>(
        UIEventListener<UIEventType::OnMouseJustPressed> listener
        )
    {
        m_eventOnMouseJustPress += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseJustReleased>(
        UIEventListener<UIEventType::OnMouseJustReleased> listener
        )
    {
        m_eventOnMouseJustRelease += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseEnter>(
        UIEventListener<UIEventType::OnMouseEnter> listener
        )
    {
        m_eventOnMouseEnter += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseLeave>(
        UIEventListener<UIEventType::OnMouseLeave> listener
        )
    {
        m_eventOnMouseLeave += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseScroll>(
        UIEventListener<UIEventType::OnMouseScroll> listener
        )
    {
        m_eventOnMouseScroll += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseClicked>(
        UIEventListener<UIEventType::OnMouseClicked> listener
        )
    {
        m_eventOnMouseClicked += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseDoubleClicked>(
        UIEventListener<UIEventType::OnMouseDoubleClicked> listener
        )
    {
        m_eventOnMouseDoubleClicked += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnFocused>(
        UIEventListener<UIEventType::OnFocused> listener
        )
    {
        m_eventOnFocused += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnUnFocused>(
        UIEventListener<UIEventType::OnUnFocused> listener
        )
    {
        m_eventOnUnFocused += listener;
    }
}