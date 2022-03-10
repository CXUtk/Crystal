#include "UIIconButton.h"

#include <Engine.h>
#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>

namespace crystal
{
    UIIconButton::UIIconButton()
    {
        m_hoverColor = Color4f(1.f);
        m_unhoverColor = Color4f(0.75f);

        m_propagationFlags = PropagationFlags::MouseScroll;
    }
    UIIconButton::~UIIconButton()
    {
    }
    void UIIconButton::MouseEnter(UIMouseEventArgs args)
    {
        m_mouseHover = true;
        UIElement::MouseEnter(args);
    }
    void UIIconButton::MouseLeave(UIMouseEventArgs args)
    {
        m_mouseHover = false;
        UIElement::MouseLeave(args);
    }
    void UIIconButton::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto device = Engine::GetInstance()->GetGraphicsDevice();

        Bound2i bound = BoundingBoxConvert<int>(m_calculatedInnerBound);
        payload.SpriteBatch->Draw(m_pIconTexture, m_calculatedInnerBound.GetCenter(), m_mouseHover ? m_hoverColor : m_unhoverColor);
    }
}
