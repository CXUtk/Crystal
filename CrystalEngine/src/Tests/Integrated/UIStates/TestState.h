#pragma once
#include <Core/UI/UIState.h>
namespace crystal
{
    class TestState : public UIState
    {
    public:
        TestState();
        virtual ~TestState() override;

        virtual void Update(const GameTimer& gameTimer) override;
        virtual void Draw(SpriteBatch* spriteBatch, const GameTimer& gameTimer) override;
    };
}