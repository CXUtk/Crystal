#pragma once

#include "../UIElement.h"
#include "../UIScrollBar.h"

namespace crystal
{
    class UIList : public UIElement
    {
    public:
        UIList();
        ~UIList() override;

        void Add(std::shared_ptr<UIElement> element);
    protected:
        std::shared_ptr<UIElement>              m_scrollViewArea = nullptr;
        std::shared_ptr<UIScrollBarV>           m_scrollBarV = nullptr;
        std::vector<std::shared_ptr<UIElement>> m_items{};

        Vector2i        m_itemsSize{};

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
        virtual void UpdateSelf(const GameTimer& gameTimer) override;
        virtual void RecalculateSelf() override;

    private:
    };
}