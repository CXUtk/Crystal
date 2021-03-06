#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"

namespace crystal
{
    class UIPanel : public UIElement
    {
    public:
        UIPanel();
        ~UIPanel() override;

        Color4f GetFillColor() const { return m_drawColor; }
        void SetFillColor(const Color4f& color) { m_drawColor = color; }

        Color4f GetBorderColor() const { return m_borderColor; }
        void SetBorderColor(const Color4f& color) { m_borderColor = color; }

    protected:
        Color4f     m_drawColor{};
        Color4f     m_borderColor{};

        std::shared_ptr<ITexture2D>     m_frameTexture = nullptr;
        std::shared_ptr<ITexture2D>     m_whiteTexture = nullptr;

        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}