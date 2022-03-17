#pragma once

#include "../UIElement.h"
#include "../UIScrollBar.h"
#include "../UILabel.h"

namespace crystal
{
    class UITabBar : public UIElement
    {
    public:
        UITabBar(const std::string& name);
        ~UITabBar() override;

        void AddOnSelectedEventListener(UIActionEvent::Func listener);

        virtual void Select();
        virtual void UnSelect();
        void MouseEnter(UIMouseEventArgs args) override;
        void MouseLeave(UIMouseEventArgs args) override;
    protected:
        virtual void RecalculateSelf() override;
        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
        //virtual void UpdateChildren(const GameTimer& gameTimer) override;
        

        UIActionEvent   m_eventOnSelected{};
        UIActionEvent   m_eventOnUnSelected{};
        bool            m_isSelected = false;
        bool            m_isHovered = false;

        std::shared_ptr<UILabel>  m_label = nullptr;
    };


    class UITabView : public UIElement
    {
    public:
        UITabView();
        ~UITabView() override;

        void AddTab(const std::string& name, std::shared_ptr<UIElement> element);
    protected:
        std::shared_ptr<UIElement>              m_container = nullptr;
        std::shared_ptr<UIElement>              m_tabContainer = nullptr;

        std::map<std::string, int>                          m_tabMapper{};
        std::vector<std::shared_ptr<UITabBar>>              m_tabBars{};
        std::vector<std::shared_ptr<UIElement>>             m_items{};

        virtual void UpdateSelf(const GameTimer& gameTimer) override;

    private:
        void SwitchTab(int index);
    };
}