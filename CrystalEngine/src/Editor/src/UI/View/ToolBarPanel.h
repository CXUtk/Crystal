#pragma once
#include <Function/UI/UIExports.h>

namespace tracer
{
    class ToolBarPanel : public crystal::UIElement
    {
    public:
        ToolBarPanel();
        ~ToolBarPanel() override;

    protected:
        virtual void DrawSelf(const crystal::RenderPayload& payload, const crystal::GameTimer& gameTimer) override;

    private:

    };
}