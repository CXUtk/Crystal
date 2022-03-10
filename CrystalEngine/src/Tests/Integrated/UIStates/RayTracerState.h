#pragma once
#include <Core/UI/UIExports.h>

namespace crystal
{
    class RayTracerState : public UIState
    {
    public:
        RayTracerState();
        virtual ~RayTracerState() override;

        virtual void Update(const GameTimer& gameTimer) override;

    private:
        std::shared_ptr<UIWidget>       m_mainWindow = nullptr;
        std::shared_ptr<UITextButton>   m_startRenderButton = nullptr;

        std::shared_ptr<UIElement>      m_body = nullptr;
        std::shared_ptr<UIElement>      m_navigator = nullptr;
        std::shared_ptr<UIElement>      m_toolBars = nullptr;
        std::shared_ptr<UIElement>      m_displayer = nullptr;
        std::shared_ptr<UIElement>      m_properties = nullptr;
    };
}