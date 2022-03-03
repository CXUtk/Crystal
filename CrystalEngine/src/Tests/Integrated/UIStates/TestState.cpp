#include "TestState.h"
#include <Core/UI/Elements/UIElement.h>
#include <Core/UI/Elements/UIPanel.h>

namespace crystal
{
    TestState::TestState()
    {
        m_uiElement = std::make_shared<UIElement>();
        m_uiElement->SetPivot(Vector2f(0.5f, 0.5f));
        m_uiElement->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        m_uiElement->SetSize(SizeLayout{ {100, 0}, {100, 0} });
        m_uiElement->AddEventListener<UIEventType::OnMouseJustPressed>([](UIMouseEventArgs args) {
            printf("Clicked\n");
        });
        m_uiElement->AddEventListener<UIEventType::OnMouseEnter>([](UIMouseEventArgs args) {
            printf("Enter\n");
        });
        m_uiElement->AddEventListener<UIEventType::OnMouseLeave>([](UIMouseEventArgs args) {
            printf("Leave\n");
        });
        m_uiElement->Recalculate();

        AppendElement(m_uiElement);


        auto uiPanel = std::make_shared<UIPanel>();
        uiPanel->SetPivot(Vector2f(0.5f, 0.5f));
        uiPanel->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        uiPanel->SetSize(SizeLayout{ {100, 0}, {100, 0} });
        uiPanel->SetPosition(Vector2f(-200, -200));
        uiPanel->Recalculate();

        AppendElement(uiPanel);
    }

    TestState::~TestState()
    {}
}