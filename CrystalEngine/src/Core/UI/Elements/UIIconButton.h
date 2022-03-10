#pragma once
#include "../Common/UICommon.h"
#include "../Common/UIEventArgs.h"

#include "UIElement.h"

namespace crystal
{
    class UIIconButton : public UIElement
    {
    public:
        UIIconButton();
        ~UIIconButton() override;

        Color4f GetMouseHoverColor() const { return m_hoverColor; }
        void SetMouseHoverColor(const Color4f& color) { m_hoverColor = color; }

        Color4f GetNormalColor() const { return m_unhoverColor; }
        void SetNormalColor(const Color4f& color) { m_unhoverColor = color; }

        std::shared_ptr<ITexture2D> GetIconTexture() const { return m_pIconTexture; }
        void SetIconTexture(std::shared_ptr<ITexture2D> texture) { m_pIconTexture = texture; }

        void MouseEnter(UIMouseEventArgs args) override;
        void MouseLeave(UIMouseEventArgs args) override;

    protected:
        Color4f     m_hoverColor{};
        Color4f     m_unhoverColor{};

        std::shared_ptr<ITexture2D>     m_pIconTexture = nullptr;

        bool    m_mouseHover = false;


        virtual void DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer) override;
    };
}