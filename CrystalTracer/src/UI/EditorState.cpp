#include "EditorState.h"

#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Core/Asset/AssetManager.h>

using namespace crystal;

namespace tracer
{
    constexpr int NAV_BAR_HEIGHT = 36;
    constexpr int TABVIEW_PADDING = 16;

    EditorState::EditorState()
    {
        m_mainWindow = std::make_shared<UIWidget>((const char*)u8"Crystal Tracer 渲染器");
        m_mainWindow->SetName("Main Window");
        m_mainWindow->SetPivot(Vector2f(0.5f, 0.5f));
        m_mainWindow->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        m_mainWindow->SetSize(SizeLayout(0, 1.f, 0, 1.f));
        m_mainWindow->SetPosition(Vector2f(0, 0));
        m_mainWindow->SetDragable(false);

        m_mainWindow->AddOnCloseEventListener([](UIEventArgs args) {
            args.Element->SetActive(false);
        });
        AppendElement(m_mainWindow);


        m_body = std::make_shared<UIElement>();
        m_body->SetName("Body");
        m_body->SetPivot(Vector2f(0.5f, 0.f));
        m_body->SetAnchorPoint(Vector2f(0.5f, 0.f));
        m_body->SetSize(SizeLayout(0, 1.f, -30, 1.f));
        m_body->SetPosition(Vector2f(0, 0));
        m_mainWindow->AppendChild(m_body);

        m_navigator = std::make_shared<UIElement>();
        m_navigator->SetName("Navigator");
        m_navigator->SetPivot(Vector2f(0.5f, 1.f));
        m_navigator->SetAnchorPoint(Vector2f(0.5f, 1.f));
        m_navigator->SetSize(SizeLayout(0, 1.f, NAV_BAR_HEIGHT, 0.f));
        m_navigator->SetPosition(Vector2f(0, 0));
        m_body->AppendChild(m_navigator);

        auto layout = std::make_shared<UILayout>();
        layout->SetName("Layout");
        layout->SetPivot(Vector2f(0.5f, 0.f));
        layout->SetAnchorPoint(Vector2f(0.5f, 0.f));
        layout->SetSize(SizeLayout(0, 1.f, -NAV_BAR_HEIGHT, 1.f));
        layout->SetPosition(Vector2f(0, 0));
        m_body->AppendChild(layout);

        m_toolBars = std::make_shared<UIElement>();
        m_toolBars->SetName("Tool Bar");
        m_toolBars->SetPivot(Vector2f(0.f, .5f));
        m_toolBars->SetAnchorPoint(Vector2f(0.f, .5f));
        m_toolBars->SetSize(SizeLayout(0, .1f, 0, 1.f));
        m_toolBars->SetPosition(Vector2f(0, 0));
        layout->AppendElement(m_toolBars, Bound2f(Vector2f(0.f), Vector2f(0.08f, 1.f)));

        m_displayer = std::make_shared<UIElement>();
        m_displayer->SetName("Displayer");
        m_displayer->SetPivot(Vector2f(0.f, .5f));
        m_displayer->SetAnchorPoint(Vector2f(0.f, .5f));
        m_displayer->SetSize(SizeLayout(0, .6f, 0, 1.f));
        m_displayer->SetPosition(Vector2f(0, 0));
        layout->AppendElement(m_displayer, Bound2f(Vector2f(.08f, 0.f), Vector2f(.75f, 1.f)));


        m_properties = std::make_shared<UIElement>();
        m_properties->SetName("Props");
        m_properties->SetPivot(Vector2f(1.f, .5f));
        m_properties->SetAnchorPoint(Vector2f(1.f, .5f));
        m_properties->SetSize(SizeLayout(0, .3f, 0, 1.f));
        m_properties->SetPosition(Vector2f(0, 0));
        layout->AppendElement(m_properties, Bound2f(Vector2f(0.75f, 0.f), Vector2f(1.f, 1.f)));

        m_renderProgressBar = std::make_shared<UIProgressBar>();
        m_renderProgressBar->SetPivot(Vector2f(0.5f, 0.f));
        m_renderProgressBar->SetAnchorPoint(Vector2f(0.5f, 0.f));
        m_renderProgressBar->SetSize(SizeLayout(-2 * TABVIEW_PADDING, 1.f, 24, 0.f));
        m_renderProgressBar->SetPosition(Vector2f(0.f, 16.f));
        m_renderProgressBar->SetValue(0.5f);
        m_displayer->AppendChild(m_renderProgressBar);

        m_displayerTabView = std::make_shared<UITabView>();
        m_displayerTabView->SetPivot(Vector2f(0.5f, 1.0f));
        m_displayerTabView->SetAnchorPoint(Vector2f(0.5f, 1.0f));
        m_displayerTabView->SetSize(SizeLayout(-2 *TABVIEW_PADDING, 1.f, -2* TABVIEW_PADDING, 0.9f));
        m_displayerTabView->SetPosition(Vector2f(0.f, -32.f));
        m_displayer->AppendChild(m_displayerTabView);

        m_displayerTabView->AddTab("Scene", std::make_shared<UIElement>());

        m_canvas = std::make_shared<Canvas>();
        m_canvas->SetPivot(Vector2f(0.5f, 0.5f));
        m_canvas->SetAnchorPoint(Vector2f(0.5f, 0.5f));
        m_canvas->SetSize(SizeLayout(0, 1.f, 0, 1.f));
        m_canvas->SetPosition(Vector2f(0.f, 0.f));
        m_displayerTabView->AddTab("Result", m_canvas);

        auto button1 = std::make_shared<UITextButton>();
        button1->SetPivot(Vector2f(0.5f, 1.f));
        button1->SetAnchorPoint(Vector2f(0.5f, 1.f));
        button1->SetPosition(Vector2f(0.f, -50.f));
        button1->SetSize(SizeLayout(72, 32));
        button1->SetText("Button");
        m_properties->AppendChild(button1);

        auto button2 = std::make_shared<UITextButton>();
        button2->SetPivot(Vector2f(0.5f, 1.f));
        button2->SetAnchorPoint(Vector2f(0.5f, 1.f));
        button2->SetPosition(Vector2f(0.f, -100.f));
        button2->SetSize(SizeLayout(72, 32));
        button2->SetText("Button");
        button2->SetEnabled(false);
        m_properties->AppendChild(button2);
    }

    EditorState::~EditorState()
    {}

    void EditorState::Update(const GameTimer & gameTimer)
    {
        UIState::Update(gameTimer);
    }
}
