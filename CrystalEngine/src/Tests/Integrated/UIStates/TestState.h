#pragma once
#include <Core/UI/UIExports.h>

namespace crystal
{
    class TestState : public UIState
    {
    public:
        TestState();
        virtual ~TestState() override;

        virtual void Update(const GameTimer& gameTimer) override;

    private:
        std::shared_ptr<UIElement>      m_uiElement = nullptr;
        std::shared_ptr<UIStyle>        m_pUIStyle = nullptr;
        std::shared_ptr<UIProgressBar>  m_progressBar = nullptr;
        std::shared_ptr<UIValueSlider>  m_slider = nullptr;
    };
}