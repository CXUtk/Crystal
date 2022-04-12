#pragma once
#include <Function/UI/UIExports.h>

namespace tracer
{
    class PropertiesPanel : public crystal::UIElement
    {
    public:
        PropertiesPanel();
        ~PropertiesPanel() override;

    protected:
        virtual void DrawSelf(const crystal::RenderPayload& payload, const crystal::GameTimer& gameTimer) override;

    private:
        std::shared_ptr<crystal::ITexture2D> m_image = nullptr;
    };
}