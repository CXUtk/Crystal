#include "UIElement.h"
#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>

namespace crystal
{
    UIElement::UIElement()
    {}

    UIElement::~UIElement()
    {}

    void UIElement::Update(const GameTimer& gameTimer)
    {
        UpdateSelf(gameTimer);
        UpdateChildren(gameTimer);
    }

    void UIElement::Draw(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        if (m_isVisible)
        {
            DrawSelf(payload, gameTimer);
        }
        if constexpr (EnableDebugDraw)
        {
            auto graphicsDevice = Engine::GetInstance()->GetGraphicsDevice();

            payload.GeometryRenderer->Begin();
            Bound2i bound = BoundingBoxConvert<int>(m_calculatedInnerBound);
            payload.GeometryRenderer->DrawBound2D(bound, Color4f(1.f, 1.f, 0.f, 1.f));
            payload.GeometryRenderer->End();
        }
        if (m_overflowStyle == OverflowStyle::Hidden)
        {
            auto RSState = payload.PSO->GetRasterState();
            auto oldScissorState = RSState->GetScissorState();
            auto oldScissorBound = RSState->GetScissorBound();

            RSState->SetScissorState(true);
            auto scissorBound = BoundingBoxConvert<int>(m_calculatedInnerBound)
                .IntersectWith(oldScissorBound);
            RSState->SetScissorBound(scissorBound);

            DrawChildren(payload, gameTimer);

            RSState->SetScissorState(oldScissorState);
            RSState->SetScissorBound(oldScissorBound);
        }
        else
        {
            DrawChildren(payload, gameTimer);
        }
    }

    void UIElement::Recalculate()
    {
        RecalculateSelf();
        RecalculateChildren();

        m_calculatedOuterBound = m_calculatedInnerBound;

        // If the child is overflow, we will combine its outer bound as ours outer bound
        // Becuase the overflowed child also respond to event
        for (auto& child : m_pChildren)
        {
            if (child->CanResponseEvent() && child->GetOverflowStyle() == OverflowStyle::Overflow)
            {
                m_calculatedOuterBound = m_calculatedOuterBound.Union(child->m_calculatedOuterBound);
            }
        }
    }

    void UIElement::AppendChild(std::shared_ptr<UIElement> element)
    {
        if (element->m_pParent)
        {
            element->m_pParent->RemoveChild(element);
        }
        element->m_pParent = this;
        m_pChildren.push_back(element);
    }

    void UIElement::RemoveChild(std::shared_ptr<UIElement> element)
    {
        auto p = std::find(m_pChildren.begin(), m_pChildren.end(), element);
        if (p == m_pChildren.end())
        {
            throw std::invalid_argument("Given UIElement does not exist in children set");
        }
        m_pChildren.erase(p);
        element->m_pParent = nullptr;
    }

    std::shared_ptr<UIElement> UIElement::GetResponseElement(const Vector2f& screenPos)
    {
        for (auto it = m_pChildren.rbegin(); it != m_pChildren.rend(); ++it)
        {
            auto& child = (*it);
            if (child->CanResponseEvent() && child->GetEventBound().Contains(screenPos))
            {
                return child->GetResponseElement(screenPos);
            }
        }
        return shared_from_this();
    }

    void UIElement::MouseJustPressed(UIMouseButtonEventArgs args)
    {
        m_eventOnMouseJustPress.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::MouseJustDown))
        {
            m_pParent->MouseJustPressed(args);
        }
    }

    void UIElement::MouseJustReleased(UIMouseButtonEventArgs args)
    {
        m_eventOnMouseJustRelease.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::MouseJustUp))
        {
            m_pParent->MouseJustReleased(args);
        }
    }

    void UIElement::MouseClicked(UIMouseButtonEventArgs args)
    {
        m_eventOnMouseClicked.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::MouseClicked))
        {
            m_pParent->MouseClicked(args);
        }
    }

    void UIElement::MouseDoubleClicked(UIMouseButtonEventArgs args)
    {
        m_eventOnMouseDoubleClicked.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::MouseDoubleClicked))
        {
            m_pParent->MouseDoubleClicked(args);
        }
    }

    void UIElement::MouseEnter(UIMouseEventArgs args)
    {
        m_eventOnMouseEnter.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::MouseEnter))
        {
            m_pParent->MouseEnter(args);
        }
    }

    void UIElement::MouseLeave(UIMouseEventArgs args)
    {
        m_eventOnMouseLeave.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::MouseLeave))
        {
            m_pParent->MouseLeave(args);
        }
    }

    void UIElement::MouseScroll(UIMouseScrollEventArgs args)
    {
        m_eventOnMouseScroll.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::MouseScroll))
        {
            m_pParent->MouseScroll(args);
        }
    }

    std::shared_ptr<UIElement> UIElement::GetChildByName(const std::string& name)
    {
        auto p = std::find_if(m_pChildren.begin(), m_pChildren.end(),
            [&](const std::shared_ptr<UIElement>& element) {
                return name == element->GetName();
            });
        if (p == m_pChildren.end())
        {
            return nullptr;
        }
        return *p;
    }

    int UIElement::GetWidth() const
    {
        return m_calculatedInnerBound.GetMaxPos().x - m_calculatedInnerBound.GetMinPos().x;
    }

    int UIElement::GetHeight() const
    {
        return m_calculatedInnerBound.GetMaxPos().y - m_calculatedInnerBound.GetMinPos().y;
    }

    void UIElement::UpdateSelf(const GameTimer& gameTimer)
    {}

    void UIElement::UpdateChildren(const GameTimer& gameTimer)
    {
        for (auto& child : m_pChildren)
        {
            if (!child->m_isActive) continue;
            child->Update(gameTimer);
        }
    }

    void UIElement::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
    }

    void UIElement::DrawChildren(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        for (auto& child : m_pChildren)
        {
            if (!child->m_isActive || !child->m_isVisible) continue;
            child->Draw(payload, gameTimer);
        }
    }

    void UIElement::RecalculateSelf()
    {
        CalculateBounds();
    }

    void UIElement::RecalculateChildren()
    {
        for (auto& child : m_pChildren)
        {
            child->Recalculate();
        }
    }

    void UIElement::CalculateBounds()
    {
        Bound2f parentBound = GetParentBound();
        auto pivotScreenPos = GetPivotScreenPos();

        auto parentSize = parentBound.GetMaxPos() - parentBound.GetMinPos();
        auto selfSize = Vector2f(m_size.Width.Relative * parentSize.x + m_size.Width.Absolute,
            m_size.Height.Relative * parentSize.y + m_size.Height.Absolute);

        m_calculatedInnerBound = Bound2f(pivotScreenPos - selfSize * m_pivot,
            pivotScreenPos + selfSize * (Vector2f(1.f) - m_pivot));
    }

    Vector2f UIElement::GetPivotScreenPos() const
    {
        Bound2f parentBound = GetParentBound();
        auto parentSize = parentBound.GetMaxPos() - parentBound.GetMinPos();
        return parentBound.GetMinPos() + parentSize * m_anchorPoint + m_position;
    }

    Bound2f UIElement::GetParentBound() const
    {
        if (m_pParent)
        {
            return m_pParent->m_calculatedInnerBound;
        }
        else
        {
            auto size = Engine::GetInstance()->GetWindow()->GetWindowSize();
            return Bound2f(Vector2f(0.f), Vector2f(size));
        }
    }
    Vector2f UIElement::GetLocalPositionToScreenPos(const Vector2f& localPos) const
    {
        Bound2f parentBound = GetParentBound();
        auto parentSize = parentBound.GetMaxPos() - parentBound.GetMinPos();
        return parentBound.GetMinPos() + parentSize * m_anchorPoint + localPos;
    }
    Vector2f UIElement::GetScreenPositionToLocalPos(const Vector2f& screenPos) const
    {
        Bound2f parentBound = GetParentBound();
        auto parentSize = parentBound.GetMaxPos() - parentBound.GetMinPos();
        auto pivotScreenPos = parentBound.GetMinPos() + parentSize * m_anchorPoint;
        return screenPos - pivotScreenPos;
    }
}
