#include "TestState.h"
#include <Core/UI/Elements/UIElement.h>

namespace crystal
{
    TestState::TestState()
    {
        m_uiElement = std::make_shared<UIElement>();
        m_uiElement->SetSize(SizeLayout{ {100, 0}, {100, 0} });
        m_uiElement->AddEventListener<UIEventType::OnMouseClicked>([](UIMouseEventArgs args) {
            printf("Clicked\n");
        });
        m_uiElement->ReCalculate();
    }
    TestState::~TestState()
    {}
    void TestState::Update(const GameTimer& gameTimer)
    {
        m_uiElement->Update(gameTimer);
    }
    void TestState::Draw(SpriteBatch* spriteBatch, const GameTimer& gameTimer)
    {
        m_uiElement->Draw(spriteBatch, gameTimer);
    }
}
