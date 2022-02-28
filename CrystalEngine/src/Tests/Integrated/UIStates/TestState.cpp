#include "TestState.h"
#include <Core/UI/Elements/UIElement.h>

namespace crystal
{
    TestState::TestState()
    {
        m_uiElement = std::make_shared<UIElement>();
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
    }

    TestState::~TestState()
    {}
}
