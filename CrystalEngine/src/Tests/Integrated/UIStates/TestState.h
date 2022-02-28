#pragma once
#include <Core/UI/UIExports.h>

namespace crystal
{
    class TestState : public UIState
    {
    public:
        TestState();
        virtual ~TestState() override;

    private:
        std::shared_ptr<UIElement> m_uiElement;
    };
}