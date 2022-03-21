#include "UIScrollBar.h"

#include <Engine.h>

#include <Core/Asset/AssetManager.h>
#include <Core/Render/RenderExports.h>
#include <Core/Input/InputController.h>
#include <Core/UI/UIStateMachine.h>

namespace crystal
{
    UIScrollBarBase::UIScrollBarBase()
    {
        m_barColor = UIStyle::GetSliderCenterColor();
        m_sliderColor = UIStyle::GetScrollBarColor();
        m_sliderHoverColor = UIStyle::GetScrollBarHoverColor();

        m_propagationFlags = PropagationFlags::MouseScroll;

        m_bar = std::make_shared<UIPanel>();
        m_bar->SetSize(SizeLayout(0.f, 1.f, 0.f, 1.f));
        m_bar->SetFillColor(m_sliderColor);
        m_bar->SetPosition(Vector2f(0.f, 0.f));
        m_bar->SetPropagationFlags(PropagationFlags::MouseScroll | PropagationFlags::MouseJustUp);

        m_bar->AddEventListener<UIEventType::OnMouseEnter>([this](UIMouseEventArgs args) {
            auto panel = static_cast<UIPanel*>(args.Element);
            panel->SetFillColor(m_sliderHoverColor);
        });

        m_bar->AddEventListener<UIEventType::OnMouseLeave>([this](UIMouseEventArgs args) {
            auto panel = static_cast<UIPanel*>(args.Element);
            panel->SetFillColor(m_sliderColor);
        });

        auto& isDragging = m_isDragging;
        auto& startOffset = m_posDragStartOffset;
        m_bar->AddEventListener<UIEventType::OnMouseJustPressed>([&isDragging, &startOffset](UIMouseButtonEventArgs args) {
            startOffset = args.Element->GetScreenPositionToLocalPos(args.MousePosScreen)
                - args.Element->GetPosition();
            isDragging = true;
        });

        m_bar->AddEventListener<UIEventType::OnMouseJustReleased>([&isDragging](UIMouseButtonEventArgs args) {
            isDragging = false;
        });

        AppendChild(m_bar);
    }


    void UIScrollBarBase::MouseJustPressed(UIMouseButtonEventArgs args)
    {
        m_isLocating = true;
    }

    void UIScrollBarBase::MouseJustReleased(UIMouseButtonEventArgs args)
    {
        m_isLocating = false;
    }

    void UIScrollBarBase::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        //auto device = Engine::GetInstance()->GetGraphicsDevice();
        //auto geometryRenderer = payload.GeometryRenderer;
        //geometryRenderer->Begin(payload.PSO);
        //geometryRenderer->DrawBound2DFill(BoundingBoxConvert<int>(m_calculatedInnerBound), m_barColor);
        //geometryRenderer->End();
        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();
        payload.SpriteBatch->Draw(stateMachine->GetWhiteTexture(), BoundingBoxConvert<int>(GetInnerBound()),
            m_barColor);
    }

    UIScrollBarV::UIScrollBarV()
        : UIScrollBarBase()
    {
        m_bar->SetPivot(Vector2f(.5f, 1.f));
        m_bar->SetAnchorPoint(Vector2f(0.5f, 1.f));
    }

    UIScrollBarV::~UIScrollBarV()
    {}

    void UIScrollBarV::UpdateSelf(const GameTimer & gameTimer)
    {
        float height = GetHeight();
        auto estimatedHeight = m_viewRange * height;
        height -= estimatedHeight;

        if (m_isDragging)
        {
            auto mousePos = Engine::GetInstance()->GetWindow()->GetMousePos();
            auto posTarget = m_bar->GetScreenPositionToLocalPos(mousePos) - Vector2f(m_posDragStartOffset);

            auto offsetY = std::clamp(posTarget.y, -height, 0.f);

            if (height == 0)
            {
                SetValue(0.f);
            }
            else
            {
                SetValue(-(double)offsetY / height);
            }
        }
        else if (m_isLocating)
        {
            auto mousePos = Engine::GetInstance()->GetWindow()->GetMousePos();
            auto estBarHeight = m_bar->GetHeight();
            auto posTarget = m_bar->GetScreenPositionToLocalPos(mousePos + Vector2i(0, estBarHeight / 2));
            auto offsetY = std::clamp(posTarget.y, -height, 0.f);

            if (height == 0)
            {
                SetValue(0.f);
            }
            else
            {
                SetValue(-(double)offsetY / height);
            }
        }

        m_bar->SetSize(SizeLayout(0, 1.f, 0.f, std::max(0.01, m_viewRange)));
        m_bar->SetPosition(Vector2f(0.f, -height * m_value));
    }

    UIScrollBarH::UIScrollBarH()
        : UIScrollBarBase()
    {
        m_bar->SetPivot(Vector2f(0.f, .5f));
        m_bar->SetAnchorPoint(Vector2f(0.f, .5f));
    }

    UIScrollBarH::~UIScrollBarH()
    {}

    void UIScrollBarH::UpdateSelf(const GameTimer & gameTimer)
    {
        auto bound = GetInnerBound();
        auto width = bound.GetSize().x;
        auto estimatedWidth = m_viewRange * width;
        width -= estimatedWidth;

        if (m_isDragging)
        {
            auto mousePos = Engine::GetInstance()->GetWindow()->GetMousePos();
            auto posTarget = m_bar->GetScreenPositionToLocalPos(mousePos) - Vector2f(m_posDragStartOffset);

            auto offsetX = std::clamp(posTarget.x, 0.f, width);

            if (width == 0)
            {
                SetValue(0.f);
            }
            else
            {
                SetValue((double)offsetX / width);
            }
        }
        else if (m_isLocating)
        {
            auto mousePos = Engine::GetInstance()->GetWindow()->GetMousePos();
            auto estBarWidth = m_bar->GetWidth();
            auto posTarget = m_bar->GetScreenPositionToLocalPos(mousePos - Vector2i(estBarWidth / 2, 0));

            auto offsetX = std::clamp(posTarget.x, 0.f, width);
            if (width == 0)
            {
                SetValue(0.f);
            }
            else
            {
                SetValue((double)offsetX / width);
            }
        }

        m_bar->SetSize(SizeLayout(0, std::max(0.01, m_viewRange), 0.f, 1.f));
        m_bar->SetPosition(Vector2f(width * m_value, 0.f));
    }
}
