#include "UIList.h"

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    static constexpr int SCROLL_WIDTH = 16;
    static constexpr int INNER_PADDING = 2;
    static constexpr int INNER_ITEM_PADDING = 2;

    UIList::UIList()
    {
        m_propagationFlags = PropagationFlags::None;

        m_listView = std::make_shared<UIListView>();
        m_listView->SetPivot(Vector2f(0.f, 0.5f));
        m_listView->SetAnchorPoint(Vector2f(0.f, 0.5f));
        m_listView->SetSize(SizeLayout(-INNER_PADDING * 3 - SCROLL_WIDTH,
            1.f, -INNER_PADDING * 2, 1.f));
        m_listView->SetPosition(Vector2f(INNER_PADDING, 0.f));

        m_scrollBarV = std::make_shared<UIScrollBarV>();
        m_scrollBarV->SetPivot(Vector2f(1.f, 0.5f));
        m_scrollBarV->SetAnchorPoint(Vector2f(1.f, 0.5f));
        m_scrollBarV->SetSize(SizeLayout(SCROLL_WIDTH, 0.f, -INNER_PADDING * 2, 1.f));
        m_scrollBarV->SetPosition(Vector2f(-INNER_PADDING, 0.f));
        m_scrollBarV->SetViewRange(.1f);

        AppendChild(m_listView);
        AppendChild(m_scrollBarV);
    }

    UIList::~UIList()
    {}

    void UIList::Add(std::shared_ptr<UIElement> element)
    {
        element->SetPivot(Vector2f(0.f, 1.f));
        element->SetAnchorPoint(Vector2f(0.f, 1.f));
        m_items.push_back(element);
        m_listView->AppendChild(element);
        element->Recalculate();
    }

    void UIList::MouseScroll(UIMouseScrollEventArgs args)
    {
        auto value = m_scrollBarV->GetValue();
        auto viewRange = m_scrollBarV->GetViewRange();
        m_scrollBarV->SetValue(std::clamp(value - args.Value.y * viewRange * 0.2, 0.0, 1.0));
    }

    void UIList::UpdateSelf(const GameTimer& gameTimer)
    {
        auto innerBound = m_listView->GetInnerBound();
        int height = innerBound.GetSize().y;
        if (m_itemsSize.y == 0)
        {
            m_scrollBarV->SetViewRange(1.0f);
        }
        else
        {
            m_scrollBarV->SetViewRange(std::min(1.0, (double)height / m_itemsSize.y));
        }

        double offsetY = std::max(0.0, m_scrollBarV->GetValue() * (m_itemsSize.y - height));

        m_itemsSize = Vector2f(0.f);
        for (auto& item : m_items)
        {
            if (!item->IsActive()) continue;
            auto itemSize = item->GetInnerBound().GetSize();
            item->SetPosition(Vector2f(0.f, -m_itemsSize.y + offsetY));
            m_itemsSize.y += itemSize.y;
            m_itemsSize.y += INNER_ITEM_PADDING;
            m_itemsSize.x = std::max(m_itemsSize.x, (int)itemSize.x);
        }
    }

    UIListView::UIListView()
    {
        m_overflowStyle = OverflowStyle::Hidden;
    }

    UIListView::~UIListView()
    {}

    void UIListView::UpdateChildren(const GameTimer& gameTimer)
    {
        int topIndex = 0;
        int botIndex = m_pChildren.size() - 1;

        for (int i = topIndex; i <= botIndex; i++)
        {
            auto& child = m_pChildren[i];
            child->Update(gameTimer);
        }
    }

    void UIListView::DrawChildren(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        int topIndex = m_pChildren.size() - 1;
        int botIndex = 0;
        int L = 0, R = m_pChildren.size() - 1;
        while (L <= R)
        {
            int mid = (L + R) / 2;
            auto& midElement = m_pChildren[mid];
            if (midElement->GetPosition().y - midElement->GetHeight() <= 0)
            {
                topIndex = mid;
                R = mid - 1;
            }
            else
            {
                L = mid + 1;
            }
        }
        L = 0, R = m_pChildren.size() - 1;
        while (L <= R)
        {
            int mid = (L + R) / 2;
            auto& midElement = m_pChildren[mid];
            if (midElement->GetPosition().y >= -GetHeight())
            {
                botIndex = mid;
                L = mid + 1;
            }
            else
            {
                R = mid - 1;
            }
        }

        for (int i = topIndex; i <= botIndex; i++)
        {
            auto& child = m_pChildren[i];
            child->Draw(payload, gameTimer);
        }
    }
}
