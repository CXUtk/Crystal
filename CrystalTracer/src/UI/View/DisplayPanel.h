#pragma once
#include <CrystalEngine/src/Core/UI/UIExports.h>
#include "Canvas.h"

namespace tracer
{
    class DisplayPanel : public crystal::UIElement
    {
    public:
        DisplayPanel();
        ~DisplayPanel() override;

    protected:
        virtual void DrawSelf(const crystal::RenderPayload& payload, const crystal::GameTimer& gameTimer) override;

    private:
        std::shared_ptr<crystal::UITabView>             m_displayerTabView = nullptr;
        std::shared_ptr<crystal::UIProgressBar>         m_renderProgressBar = nullptr;
        std::shared_ptr<Canvas>                         m_canvas = nullptr;
    };
}