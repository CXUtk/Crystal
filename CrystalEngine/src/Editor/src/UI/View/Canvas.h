#pragma once
#include <Function/UI/UIExports.h>

namespace tracer
{
    class Canvas : public crystal::UIElement
    {
    public:
        Canvas();
        ~Canvas() override;

    protected:
        virtual void DrawSelf(const crystal::RenderPayload& payload, const crystal::GameTimer& gameTimer) override;

    private:
        std::shared_ptr<crystal::ITexture2D> m_image = nullptr;
    };
}