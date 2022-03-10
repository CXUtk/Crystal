#pragma once

#include "../UIElement.h"
#include "../UIScrollBar.h"

namespace crystal
{
    class UIListView : public UIElement
    {
    public:
        UIListView();
        ~UIListView() override;

    protected:
        virtual void UpdateChildren(const GameTimer& gameTimer) override;
        virtual void DrawChildren(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };


    class UIList : public UIElement
    {
    public:
        UIList();
        ~UIList() override;

        void Add(std::shared_ptr<UIElement> element);
        virtual void MouseScroll(UIMouseScrollEventArgs args) override;
    protected:
        std::shared_ptr<UIListView>             m_listView = nullptr;
        std::shared_ptr<UIScrollBarV>           m_scrollBarV = nullptr;
        std::vector<std::shared_ptr<UIElement>> m_items{};

        Vector2i        m_itemsSize{};

        virtual void UpdateSelf(const GameTimer& gameTimer) override;

    private:
    };
}