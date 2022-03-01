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
        DrawSelf(payload, gameTimer);
        DrawChildren(payload, gameTimer);
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
            if (child->IsActive() && child->GetOverflowStyle() == OverflowStyle::Overflow)
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
        for (auto& child : m_pChildren)
        {
            if (child->CanResponseEvent() && child->GetEventBound().Contains(screenPos))
            {
                return child->GetResponseElement(screenPos);
            }
        }
        return shared_from_this();
    }

    void UIElement::MouseJustPressed(UIMouseButtonEventArgs args)
    {
        if (args.ButtonFlags != MouseButtonFlags::None)
        {
            m_eventOnMouseJustPress.Invoke(args);
        }
    }

    void UIElement::MouseJustReleased(UIMouseButtonEventArgs args)
    {
        if (args.ButtonFlags != MouseButtonFlags::None)
        {
            m_eventOnMouseJustRelease.Invoke(args);
        }
    }

    void UIElement::MouseEnter(UIMouseEventArgs args)
    {
        m_eventOnMouseEnter.Invoke(args);
    }

    void UIElement::MouseLeave(UIMouseEventArgs args)
    {
        m_eventOnMouseLeave.Invoke(args);
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
        if constexpr (EnableDebugDraw)
        {
            auto graphicsDevice = Engine::GetInstance()->GetGraphicsDevice();

            payload.GeometryRenderer->Begin();
            Bound2i bound = BoundingBoxConvert<int>(m_calculatedInnerBound);
            payload.GeometryRenderer->DrawBound2D(bound, Color4f(1.f, 1.f, 0.f, 1.f));
            payload.GeometryRenderer->End();
        }
    }

    void UIElement::DrawChildren(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        for (auto it = m_pChildren.rbegin(); it != m_pChildren.rend(); ++it)
        {
            auto& child = (*it);
            if (!child->m_isActive || !child->m_isVisible) continue;
            (*it)->Draw(payload, gameTimer);
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
        Bound2f parentBound = {};
        if (m_pParent)
        {
            parentBound = m_pParent->m_calculatedInnerBound;
        }
        else
        {
            auto size = Engine::GetInstance()->GetWindow()->GetWindowSize();
            parentBound = Bound2f(Vector2f(0.f), Vector2f(size));
        }
        auto parentSize = parentBound.GetMaxPos() - parentBound.GetMinPos();
        auto pivotPosWorld = parentBound.GetMinPos() + parentSize * m_anchorPoint;

        auto selfSize = Vector2f(m_size.Width.Relative * parentSize.x + m_size.Width.Absolute,
            m_size.Height.Relative * parentSize.y + m_size.Height.Absolute);

        m_calculatedInnerBound = Bound2f(pivotPosWorld - selfSize * m_pivot,
            pivotPosWorld + selfSize * (Vector2f(1.f) - m_pivot));
    }
}
