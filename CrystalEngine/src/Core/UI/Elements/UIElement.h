#pragma once
#include "../UICommon.h"
#include "../UIEventArgs.h"

namespace crystal
{
    class UIElement : public std::enable_shared_from_this<UIElement>
    {
    public:
        UIElement();
        virtual ~UIElement();

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(SpriteBatch* spriteBatch, const GameTimer& gameTimer);
        virtual void ReCalculate();

        // Add Listeners
        template<UIEventType E>
        void AddEventListener(UIEventListener<E> listener);

        void AppendChild(std::shared_ptr<UIElement> element);
        void RemoveChild(std::shared_ptr<UIElement> element);
        std::shared_ptr<UIElement> GetChildByName(const std::string& name);

        std::string GetName() const { return m_name; }

        Vector2f GetPivot() const { return m_pivot; }
        void SetPivot(Vector2f pivot) { m_pivot = pivot; }

        Vector2f GetPosition() const { return m_position; }
        void SetPosition(Vector2f pos) { m_position = pos; }

        Vector2f GetAnchorPoint() const { return m_anchorPoint; }
        void SetAnchorPoint(Vector2f anchorPoint) { m_anchorPoint = anchorPoint; }

        SizeLayout GetSize() const { return m_size; }
        void SetSize(SizeLayout size) { m_size = size; }
    protected:
        // Virtual
        virtual void UpdateSelf(const GameTimer& gameTimer);
        virtual void UpdateChildren(const GameTimer& gameTimer);
        virtual void DrawSelf(SpriteBatch* spriteBatch, const GameTimer& gameTimer);
        virtual void DrawChildren(SpriteBatch* spriteBatch, const GameTimer& gameTimer);

        /// Data

        // Events
        UIEvent<UIEventType::OnMouseClicked>    m_eventOnMouseClick{};
        UIEvent<UIEventType::OnMouseDown>       m_eventOnMouseDown{};
        UIEvent<UIEventType::OnMouseUp>         m_eventOnMouseUp{};

        // Relationship
        std::vector<std::shared_ptr<UIElement>>     m_pChildren{};
        UIElement*                                  m_pParent = nullptr;

        // Properties
        bool        m_isActive = true;
        bool        m_isVisible = true;
        bool        m_noEvent = false;
        bool        m_isFocused = false;

        std::string     m_name{};
        std::string     m_tooltip{};

        OverflowStyle       m_overflowStyle = OverflowStyle::Overflow;
        PropagationFlags    m_propagationFlags = PropagationFlags::None;

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


        void CalculateBounds();
    };

    template<UIEventType E>
    inline void UIElement::AddEventListener(UIEventListener<E> listener)
    {
        static_assert(false, "Unknown UI Event Listener Type");
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseClicked>(
        UIEventListener<UIEventType::OnMouseClicked> listener
        )
    {
        m_eventOnMouseClick += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseDown>(
        UIEventListener<UIEventType::OnMouseDown> listener
        )
    {
        m_eventOnMouseDown += listener;
    }

    template<>
    inline void UIElement::AddEventListener<UIEventType::OnMouseUp>(
        UIEventListener<UIEventType::OnMouseUp> listener
        )
    {
        m_eventOnMouseUp += listener;
    }
}