#include "UIScrollBar.h"

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>

namespace crystal
{
    UIScrollBarV::UIScrollBarV()
    {
        m_barColor = UIStyle::GetDarkerBarColor();
        m_sliderColor = UIStyle::GetButtonColor();
        m_sliderHoverColor = UIStyle::GetButtonHoverColor();

        m_bar = std::make_shared<UIPanel>();
        m_bar->SetPivot(Vector2f(.5f, 1.f));
        m_bar->SetAnchorPoint(Vector2f(0.5f, 1.f));
        m_bar->SetSize(SizeLayout(0.f, 1.f, 0.f, 1.f));
        m_bar->SetFillColor(m_sliderColor);
        m_bar->SetPosition(Vector2f(0.f, 0.f));

        m_bar->AddEventListener<UIEventType::OnMouseEnter>([this](UIMouseEventArgs args) {
            auto panel = static_cast<UIPanel*>(args.Element);
            panel->SetFillColor(m_sliderHoverColor);
        });

        m_bar->AddEventListener<UIEventType::OnMouseLeave>([this](UIMouseEventArgs args) {
            auto panel = static_cast<UIPanel*>(args.Element);
            panel->SetFillColor(m_sliderColor);
        });

        AppendChild(m_bar);

        m_propagationFlags = PropagationFlags::MouseScroll;
    }

    UIScrollBarV::~UIScrollBarV()
    {}


    void UIScrollBarV::MouseJustPressed(UIMouseButtonEventArgs args)
    {
        m_isDragging = true;
    }

    void UIScrollBarV::MouseJustReleased(UIMouseButtonEventArgs args)
    {
        m_isDragging = false;
    }

    void UIScrollBarV::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto device = Engine::GetInstance()->GetGraphicsDevice();
        auto geometryRenderer = payload.GeometryRenderer;
        geometryRenderer->Begin(payload.PSO);
        geometryRenderer->DrawBound2DFill(BoundingBoxConvert<int>(m_calculatedInnerBound), m_barColor);
        geometryRenderer->End();
    }

    void UIScrollBarV::UpdateSelf(const GameTimer& gameTimer)
    {
        if (m_isDragging)
        {
            auto mousePos = Engine::GetInstance()->GetWindow()->GetMousePos();
            auto posTarget = m_bar->GetScreenPositionToLocalPos(mousePos);

            auto height = m_calculatedInnerBound.GetSize().y;

            auto offsetX = std::clamp(posTarget.y, -height, 0.f);

            SetValue(-(double)offsetX / height);
        }
    }

    void UIScrollBarV::RecalculateChildren()
    {
        m_value = std::clamp(m_value, 0.0, 1.0);

        auto minPos = m_calculatedInnerBound.GetMinPos();
        auto maxPos = m_calculatedInnerBound.GetMaxPos();

        m_bar->SetSize(SizeLayout(0, 1.f, 0.f, m_viewRange));

        auto height = m_calculatedInnerBound.GetSize().y;

        auto estimatedHeight = m_viewRange * height;

        m_bar->SetPosition(Vector2f(0.f, -(height - estimatedHeight) * m_value));
        UIElement::RecalculateChildren();
    }


}
