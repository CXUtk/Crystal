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

        m_scrollViewArea = std::make_shared<UIElement>();
        m_scrollViewArea->SetPivot(Vector2f(0.f, 0.5f));
        m_scrollViewArea->SetAnchorPoint(Vector2f(0.f, 0.5f));
        m_scrollViewArea->SetSize(SizeLayout(-INNER_PADDING * 3 - SCROLL_WIDTH,
            1.f, -INNER_PADDING * 2, 1.f));
        m_scrollViewArea->SetPosition(Vector2f(INNER_PADDING, 0.f));
        m_scrollViewArea->SetOverflowStyle(OverflowStyle::Hidden);

        m_scrollBarV = std::make_shared<UIScrollBarV>();
        m_scrollBarV->SetPivot(Vector2f(1.f, 0.5f));
        m_scrollBarV->SetAnchorPoint(Vector2f(1.f, 0.5f));
        m_scrollBarV->SetSize(SizeLayout(SCROLL_WIDTH, 0.f, -INNER_PADDING * 2, 1.f));
        m_scrollBarV->SetPosition(Vector2f(-INNER_PADDING, 0.f));
        m_scrollBarV->SetViewRange(.1f);

        AppendChild(m_scrollViewArea);
        AppendChild(m_scrollBarV);
    }

    UIList::~UIList()
    {}

    void UIList::Add(std::shared_ptr<UIElement> element)
    {
        element->SetPivot(Vector2f(0.f, 1.f));
        element->SetAnchorPoint(Vector2f(0.f, 1.f));
        m_items.push_back(element);
        m_scrollViewArea->AppendChild(element);
    }

    void UIList::DrawSelf(const RenderPayload & payload, const GameTimer & gameTimer)
    {
        if (true)
        {

        }
    }

    void UIList::UpdateSelf(const GameTimer& gameTimer)
    {
        m_itemsSize = Vector2i(0);
        
        for (auto& item : m_items)
        {
            auto itemSize = item->GetEstimatedSize(ptr(m_scrollViewArea));
            item->SetPosition(Vector2f(0.f, -m_itemsSize.y));
            m_itemsSize.y += itemSize.y;
            m_itemsSize.y += INNER_ITEM_PADDING;
            m_itemsSize.x = std::max(m_itemsSize.x, itemSize.x);
        }
        auto innerBound = m_scrollViewArea->GetInnerBound();
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
        float posY = 0.f;
        for (auto& item : m_items)
        {
            item->SetPosition(Vector2f(0.f, -posY + offsetY));
            auto itemSize = item->GetEstimatedSize(ptr(m_scrollViewArea));
            posY += itemSize.y;
            posY += INNER_ITEM_PADDING;
        }
    }

    void UIList::RecalculateSelf()
    {
        for (auto& item : m_items)
        {
            item->Recalculate();
        }
    }
}
