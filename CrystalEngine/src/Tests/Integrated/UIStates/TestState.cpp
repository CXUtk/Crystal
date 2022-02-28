#include "TestState.h"
#include <Core/UI/Elements/UIElement.h>

namespace crystal
{
    TestState::TestState()
    {
        UIElement element;
        element.AddEventListener<UIEventType::OnMouseClicked>([](UIMouseEventArgs args) {
            printf("Clicked\n");
        });
    }
    TestState::~TestState()
    {}
    void TestState::Update(const GameTimer& gameTimer)
    {}
    void TestState::Draw(const GameTimer& gameTimer)
    {}
}
