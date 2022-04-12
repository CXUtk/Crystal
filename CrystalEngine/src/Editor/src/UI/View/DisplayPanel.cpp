#include "DisplayPanel.h"
#include <Engine.h>

#include <Resource/Asset/AssetManager.h>
#include <Function/Render/RenderExports.h>
#include <Function/Input/InputController.h>
#include <Function/UI/UIStateMachine.h>

using namespace crystal;

namespace tracer
{
    constexpr int TABVIEW_PADDING = 16;

    DisplayPanel::DisplayPanel()
    {
        m_renderProgressBar = std::make_shared<UIProgressBar>();
        m_renderProgressBar->SetName("Progress");
        m_renderProgressBar->SetPivot(Vector2f(0.5f, 0.f));
        m_renderProgressBar->SetAnchorPoint(Vector2f(0.5f, 0.f));
        m_renderProgressBar->SetSize(SizeLayout(-2 * TABVIEW_PADDING - 4, 1.f, 24, 0.f));
        m_renderProgressBar->SetPosition(Vector2f(0.f, 32.f));
        m_renderProgressBar->SetValue(0.5f);
        AppendChild(m_renderProgressBar);
         
        m_displayerTabView = std::make_shared<UITabView>();
        m_displayerTabView->SetPivot(Vector2f(0.5f, 1.0f));
        m_displayerTabView->SetAnchorPoint(Vector2f(0.5f, 1.0f));
        m_displayerTabView->SetSize(SizeLayout(-2 * TABVIEW_PADDING, 1.f, -2 * TABVIEW_PADDING, 0.9f));
        m_displayerTabView->SetPosition(Vector2f(0.f, -32.f));
        AppendChild(m_displayerTabView);

        m_displayerTabView->AddTab("Scene", std::make_shared<UIElement>());

        m_canvas = std::make_shared<Canvas>();
        m_canvas->SetPivot(Vector2f(0.5f, 0.5f));
        m_canvas->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        m_canvas->SetSize(SizeLayout(0, 1.f, 0, 1.f));
        m_canvas->SetPosition(Vector2f(0.f, 0.f));
        m_displayerTabView->AddTab("Result", m_canvas);
    }
    DisplayPanel::~DisplayPanel()
    {}
    void DisplayPanel::DrawSelf(const crystal::RenderPayload & payload, const crystal::GameTimer & gameTimer)
    {}
}