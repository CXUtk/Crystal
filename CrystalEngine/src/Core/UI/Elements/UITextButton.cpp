#include "UITextButton.h"

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    UITextButton::UITextButton()
    {
        m_unhoverColor = UIStyle::GetButtonColor();
        m_hoverColor = UIStyle::GetButtonHoverColor();

        m_penel = std::make_shared<UIPanel>();
        m_penel->SetPivot(Vector2f(0.f));
        m_penel->SetAnchorPoint(Vector2f(0.f));
        m_penel->SetSize(SizeLayout(0, 1.f, 0, 1.f));
        m_penel->SetFillColor(m_unhoverColor);
        m_penel->SetNoEvents(true);

        m_buttonText = std::make_shared<UILabel>();
        m_buttonText->SetPivot(Vector2f(0.5f));
        m_buttonText->SetAnchorPoint(Vector2f(0.5f));
        m_buttonText->SetNoEvents(true);

        AppendChild(m_penel);
        AppendChild(m_buttonText);
    }

    UITextButton::~UITextButton()
    {}

    void UITextButton::MouseEnter(UIMouseEventArgs args)
    {
        m_penel->SetFillColor(m_hoverColor);
        m_isMouseHover = true;
        UIElement::MouseEnter(args);
    }

    void UITextButton::MouseLeave(UIMouseEventArgs args)
    {
        m_penel->SetFillColor(m_unhoverColor);
        m_isMouseHover = false;
        UIElement::MouseLeave(args);
    }


    void UITextButton::DrawSelf(const RenderPayload & payload, const GameTimer & gameTimer)
    {}
}
