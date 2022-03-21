#include "UITabView.h"

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>
#include <Core/UI/UIStateMachine.h>

namespace crystal
{
    constexpr int LABEL_PADDING_BOT = 2;
    constexpr int LABEL_PADDING_LEFT = 5;
    constexpr int LABEL_PADDING_RIGHT = 8;
    UITabBar::UITabBar(const std::string& name)
    {
        m_propagationFlags = PropagationFlags::MouseScroll;

        m_label = std::make_shared<UILabel>(name);
        m_label->SetPivot(Vector2f(0.f, 0.f));
        m_label->SetAnchorPoint(Vector2f(0.f, 0.f));
        m_label->SetPosition(Vector2f(LABEL_PADDING_LEFT, LABEL_PADDING_BOT));
        AppendChild(m_label);

        SetSize(SizeLayout(0, 0.f, 0, 1.f));

        m_dependOnChildrenWidth = true;
    }

    UITabBar::~UITabBar()
    {}

    void UITabBar::AddOnSelectedEventListener(UIActionEvent::Func listener)
    {
        m_eventOnSelected += listener;
    }

    void UITabBar::Select()
    {
        m_isSelected = true;
        UIEventArgs args = {};
        args.Element = this;
        args.TimeStamp = 0.0;
        m_eventOnSelected.Invoke(args);
    }

    void UITabBar::UnSelect()
    {
        m_isSelected = false;
        UIEventArgs args = {};
        args.Element = this;
        args.TimeStamp = 0.0;
        m_eventOnUnSelected.Invoke(args);
    }

    void UITabBar::MouseEnter(UIMouseEventArgs args)
    {
        m_isHovered = true;
    }

    void UITabBar::MouseLeave(UIMouseEventArgs args)
    {
        m_isHovered = false;
    }

    void UITabBar::RecalculateWidth()
    {
        UIElement::RecalculateWidth();
        m_calculatedWidth = LABEL_PADDING_LEFT + LABEL_PADDING_RIGHT + m_label->GetWidth();
    }

    void UITabBar::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto spriteBatch = payload.SpriteBatch;
        SliceInfo slice = {};
        slice.Left = 1;
        slice.Right = 1;
        slice.Top = 1;
        slice.Bot = 1;
        slice.DrawFlags = Slice_Nine;

        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();

        auto bound = BoundingBoxConvert<int>(GetInnerBound());
        spriteBatch->Draw(stateMachine->GetWhiteTexture(), bound, (m_isSelected || m_isHovered) ? UIStyle::GetButtonHoverColor() : UIStyle::GetButtonColor());
        //spriteBatch->DrawSlicedTexture(stateMachine->GetFrameTexture(), slice, bound, (m_isSelected || m_isHovered) ? UIStyle::GetPanelBorderColorHighlight() : UIStyle::GetPanelBorderColor());
    }


    constexpr int CONTAINER_PADDING = 2;
    constexpr int TAB_BAR_PADDING = 1;
    constexpr int TAB_BAR_HEIGHT = 32;
    UITabView::UITabView()
    {
        m_propagationFlags = PropagationFlags::MouseScroll;

        m_container = std::make_shared<UIElement>();
        m_container->SetPivot(Vector2f(0.5f, 0.f));
        m_container->SetAnchorPoint(Vector2f(0.5f, 0.f));
        m_container->SetSize(SizeLayout(-CONTAINER_PADDING * 2,
            1.f, -CONTAINER_PADDING - TAB_BAR_HEIGHT, 1.f));
        m_container->SetPosition(Vector2f(0.f, CONTAINER_PADDING));
        AppendChild(m_container);

        m_tabContainer = std::make_shared<UIElement>();
        m_tabContainer->SetPivot(Vector2f(0.5f, 1.f));
        m_tabContainer->SetAnchorPoint(Vector2f(0.5f, 1.f));
        m_tabContainer->SetSize(SizeLayout(-CONTAINER_PADDING * 2,
            1.f, TAB_BAR_HEIGHT - CONTAINER_PADDING, 0.f));
        m_tabContainer->SetPosition(Vector2f(0.f, -CONTAINER_PADDING));
        AppendChild(m_tabContainer);
    }

    UITabView::~UITabView()
    {}

    void UITabView::AddTab(const std::string& name, std::shared_ptr<UIElement> element)
    {
        auto tabBar = std::make_shared<UITabBar>(name);
        tabBar->SetPivot(Vector2f(0.f, 0.f));
        tabBar->SetAnchorPoint(Vector2f(0.f, 0.f));

        int tabIndex = m_tabBars.size();
        tabBar->AddEventListener<UIEventType::OnMouseClicked>([this, tabIndex](UIMouseButtonEventArgs args) {
            SwitchTab(tabIndex);
        });

        bool firstTab = m_tabBars.empty();

        m_tabMapper[name] = tabIndex;
        m_tabBars.push_back(tabBar);
        m_items.push_back(element);

        m_tabContainer->AppendChild(tabBar);

        if (firstTab)
        {
            SwitchTab(0);
        }
    }

    void UITabView::UpdateSelf(const GameTimer& gameTimer)
    {
        int size = m_tabBars.size();
        float offsetX = 0.f;
        for (int i = 0; i < size; i++)
        {
            auto& bar = m_tabBars[i];
            bar->SetPosition(Vector2f(offsetX, 0.f));
            offsetX += bar->GetWidth();
            if (i != size - 1)
            {
                offsetX += TAB_BAR_PADDING;
            }
        }
    }

    void UITabView::DrawChildren(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        UIElement::DrawChildren(payload, gameTimer);
        auto spriteBatch = payload.SpriteBatch;
        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();

        auto tabBound = BoundingBoxConvert<int>(m_tabContainer->GetInnerBound());
        auto bound = Bound2i(tabBound.GetMinPos() - Vector2i(0, 2), tabBound.GetMinPos() + Vector2i(tabBound.GetSize().x, 1));
        spriteBatch->Draw(stateMachine->GetWhiteTexture(), bound, UIStyle::GetButtonHoverColor());
    }

    void UITabView::SwitchTab(int index)
    {
        for (auto& tab : m_tabBars)
        {
            tab->UnSelect();
        }
        m_tabBars[index]->Select();
        m_container->RemoveAllChildren();
        m_container->AppendChild(m_items[index]);
    }
}