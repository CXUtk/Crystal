#include "UIValueSlider.h"

#include <Engine.h>

#include "Resource/Asset/AssetManager.h"

#include "Function/Render/RenderExports.h"
#include "Function/Input/InputController.h"
#include "Function/UI/UIStateMachine.h"

namespace crystal
{
    static constexpr int BAR_WIDTH = 5;
    static constexpr int BAR_HEIGHT_PADDING = 1;

    UIValueSlider::UIValueSlider()
    {
        m_barColor = UIStyle::GetSliderCenterColor();
        m_sliderColor = UIStyle::GetScrollBarColor();
        m_sliderHoverColor = UIStyle::GetScrollBarHoverColor();

        m_bar = std::make_shared<UIPanel>();
        m_bar->SetPivot(Vector2f(0.5f));
        m_bar->SetAnchorPoint(Vector2f(0.f, 0.5f));
        m_bar->SetSize(SizeLayout(BAR_WIDTH, 0.f, -2 * BAR_HEIGHT_PADDING, 1.f));
        m_bar->SetFillColor(m_sliderColor);
        m_bar->SetPosition(Vector2f(0.f, 0.f));
        m_bar->SetPropagationFlags(PropagationFlags::All);

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
    UIValueSlider::~UIValueSlider()
    {}


    void UIValueSlider::MouseJustPressed(UIMouseButtonEventArgs args)
    {
        m_isDragging = true;
    }

    void UIValueSlider::MouseJustReleased(UIMouseButtonEventArgs args)
    {
        m_isDragging = false;
    }

    void UIValueSlider::DrawSelf(const RenderPayload& payload, const GameTimer& gameTimer)
    {
        auto spriteBatch = payload.SpriteBatch;

        auto bound = GetInnerBound();
        auto minPos = bound.GetMinPos();
        auto maxPos = bound.GetMaxPos();

        auto height = bound.GetSize().y;
        auto width = bound.GetSize().x - BAR_WIDTH * 2;
        auto midStartPos = minPos + Vector2f(BAR_WIDTH, height * 0.5f);

        auto barBound = Bound2f(midStartPos - Vector2f(0, BAR_WIDTH * 0.5f),
            midStartPos + Vector2f(width, BAR_WIDTH * 0.5f));

        auto stateMachine = Engine::GetInstance()->GetUIStateMachine();

        spriteBatch->Draw(stateMachine->GetWhiteTexture(), BoundingBoxConvert<int>(barBound),
            m_barColor);
    }

    void UIValueSlider::UpdateSelf(const GameTimer& gameTimer)
    {
        if (m_isDragging)
        {
            auto mousePos = Engine::GetInstance()->GetWindow()->GetMousePos();
            auto posTarget = m_bar->GetScreenPositionToLocalPos(mousePos);

            float width = GetWidth() - BAR_WIDTH * 2;

            auto offsetX = std::clamp(posTarget.x - BAR_WIDTH, 0.f, width);

            SetValue(std::clamp((double)offsetX / width, 0.0, 1.0));

            m_isStateDirty = true;
        }

        auto width = GetWidth() - BAR_WIDTH * 2;
        m_bar->SetPosition(Vector2f(BAR_WIDTH + width * m_value, 0.f));
    }
}
