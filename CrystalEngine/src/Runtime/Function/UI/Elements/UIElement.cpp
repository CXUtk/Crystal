#include "UIElement.h"
#include <Engine.h>

#include "Resource/Asset/AssetManager.h"
#include "Function/Render/RenderExports.h"
#include "Function/UI/UIStateMachine.h"

namespace crystal
{
    UIElement::UIElement()
    {}

    UIElement::~UIElement()
    {}

    void UIElement::SetPosition(Vector2f pos)
    {
        if (m_position != pos)
        {
            m_isStateDirty = true;
        }
        m_position = pos;
    }

    void UIElement::SetPivot(Vector2f pivot)
    {
        if (m_pivot != pivot)
        {
            m_isStateDirty = true;
        }
        m_pivot = pivot;
    }

    void UIElement::SetAnchorPoint(Vector2f anchorPoint)
    {
        if (m_anchorPoint != anchorPoint)
        {
            m_isStateDirty = true;
        }
        m_anchorPoint = anchorPoint;
    }

    void UIElement::SetSize(SizeLayout size)
    {
        if (size.Width != m_size.Width || size.Height != m_size.Height)
        {
            m_isStateDirty = true;
        }
        m_size = size;
    }

    void UIElement::Update(const GameTimer& gameTimer)
    {
        if (!m_isEnabled) return;
        UpdateSelf(gameTimer);


        UIUpdateEventArgs args = {};
        args.Element = this;
        args.TimeStamp = gameTimer.GetCurrentTime();
        args.GameTimer = &gameTimer;
        m_eventPostUpdate.Invoke(args);

        UpdateChildren(gameTimer);
    }

    void UIElement::Draw(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto spriteBatch = payload.SpriteBatch;
        if (m_isStateDirty)
        {
            Recalculate();
        }
        auto innerBound = GetInnerBound();

        if (m_isVisible)
        {
            DrawSelf(payload, gameTimer);

            UIDrawEventArgs args = {};
            args.Element = this;
            args.TimeStamp = gameTimer.GetCurrentTime();
            args.GameTimer = &gameTimer;
            args.Payload = &payload;
            m_eventPostDraw.Invoke(args);
        }
        if constexpr (EnableDebugDraw)
        {
            auto stateMachine = Engine::GetInstance()->GetUIStateMachine();
            SliceInfo slice = {};
            slice.Left = 1;
            slice.Right = 1;
            slice.Top = 1;
            slice.Bot = 1;
            slice.DrawFlags = Slice_Nine;

            spriteBatch->DrawSlicedTexture(stateMachine->GetFrameTexture(), slice,
                BoundingBoxConvert<int>(innerBound), Color4f(1.f, 1.f, 0.f, 1.f));
        }
        if (m_overflowStyle == OverflowStyle::Hidden)
        {
            spriteBatch->End();

            auto RSState = payload.PSO->GetRasterState();
            auto oldScissorState = RSState->GetScissorState();
            auto oldScissorBound = RSState->GetScissorBound();

            RSState->SetScissorState(true);
            auto scissorBound = BoundingBoxConvert<int>(innerBound)
                .IntersectWith(oldScissorBound);
            RSState->SetScissorBound(scissorBound);

            spriteBatch->Begin(SpriteSortMode::Deferred, payload.PSO);

            DrawChildren(payload, gameTimer);

            spriteBatch->End();

            RSState->SetScissorState(oldScissorState);
            RSState->SetScissorBound(oldScissorBound);

            spriteBatch->Begin(SpriteSortMode::Deferred, payload.PSO);
        }
        else
        {
            DrawChildren(payload, gameTimer);
        }
    }

    void UIElement::Recalculate(RecalculateMask mask)
    {
#ifdef _DEBUG
        bool widthDependant = false;
        bool heightDependant = false;
        for (auto& child : m_pChildren)
        {
            if (child->IsActive())
            {
                if (child->m_size.Width.Relative != 0.f)
                {
                    widthDependant = true;
                }
                if (child->m_size.Height.Relative != 0.f)
                {
                    heightDependant = true;
                }
            }
        }
        // Should not depend on both parent width and children width
        assert(!(m_dependOnChildrenWidth && widthDependant));
        // Should not depend on both parent height and children height
        assert(!(m_dependOnChildrenHeight && heightDependant));
#endif

        RecalculateMask next_mask = RecalculateMask::None;
        if (m_dependOnChildrenWidth)
        {
            next_mask = next_mask | RecalculateMask::Mask_Width;
        }
        if (m_dependOnChildrenHeight)
        {
            next_mask = next_mask | RecalculateMask::Mask_Height;
        }

        if (!(mask & RecalculateMask::Mask_Width))
        {
            RecalculateWidth();
        }
        if (!(mask & RecalculateMask::Mask_Width))
        {
            RecalculateHeight();
        }
        RecalculatePosition();

        RecalculateSelf();

        RecalculateChildren(next_mask);

        //m_calculatedOuterBound = m_calculatedInnerBound;

        //// If the child is overflow, we will combine its outer bound as ours outer bound
        //// Becuase the overflowed child also respond to event
        //for (auto& child : m_pChildren)
        //{
        //    if (child->CanResponseEvent() && m_overflowStyle == OverflowStyle::Overflow)
        //    {
        //        m_calculatedOuterBound = m_calculatedOuterBound.Union(child->m_calculatedOuterBound);
        //    }
        //}

        m_isStateDirty = false;
    }

    void UIElement::RecalculateWidth()
    {
        if (m_dependOnChildrenWidth)
        {
            for (auto& child : m_pChildren)
            {
                if (child->IsActive())
                {
                    child->RecalculateWidth();
                }
            }
        }
        else
        {
            auto parentSize = GetParentBound().GetSize();
            m_calculatedWidth = parentSize.x * m_size.Width.Relative + m_size.Width.Absolute;
        }
    }

    void UIElement::RecalculateHeight()
    {
        if (m_dependOnChildrenHeight)
        {
            for (auto& child : m_pChildren)
            {
                if (child->IsActive())
                {
                    child->RecalculateHeight();
                }
            }
        }
        else
        {
            auto parentSize = GetParentBound().GetSize();
            m_calculatedHeight = parentSize.y * m_size.Height.Relative + m_size.Height.Absolute;
        }
    }

    void UIElement::RecalculatePosition()
    {
        auto pivotScreenPos = GetPivotScreenPos();
        auto selfSize = Vector2f(m_calculatedWidth, m_calculatedHeight);
        m_calculatedBotLeft = pivotScreenPos - selfSize * m_pivot;
    }

    void UIElement::AppendChild(std::shared_ptr<UIElement> element)
    {
        if (element->m_pParent && element->m_pParent != this)
        {
            element->m_pParent->RemoveChild(element);
        }
        element->m_pParent = this;
        m_pChildren.push_back(element);
        //element->Recalculate();
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

    void UIElement::RemoveAllChildren()
    {
        for (auto& child : m_pChildren)
        {
            child->m_pParent = nullptr;
        }
        m_pChildren.clear();
    }

    std::shared_ptr<UIElement> UIElement::GetResponseElement(const Vector2f& screenPos)
    {
        for (auto it = m_pChildren.rbegin(); it != m_pChildren.rend(); ++it)
        {
            auto& child = (*it);
            if (child->CanResponseEvent() && child->GetInnerBound().Contains(screenPos))
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

    void UIElement::OnFocused(UIEventArgs args)
    {
        m_eventOnFocused.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::Focus))
        {
            m_pParent->OnFocused(args);
        }
    }

    void UIElement::OnUnFocused(UIEventArgs args)
    {
        m_eventOnUnFocused.Invoke(args);
        if (m_pParent && (m_propagationFlags & PropagationFlags::UnFocus))
        {
            m_pParent->OnUnFocused(args);
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
    }

    void UIElement::RecalculateChildren(RecalculateMask mask)
    {
        if (!m_dependOnChildrenWidth)
        {
            for (auto& child : m_pChildren)
            {
                if (child->IsActive())
                {
                    child->Recalculate(mask);
                }
            }
        }
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
            return m_pParent->GetInnerBound();
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
