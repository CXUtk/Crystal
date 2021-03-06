#include "EditorState.h"

#include <Engine.h>
#include <Resource/Asset/AssetManager.h>

using namespace crystal;

namespace tracer
{
    constexpr int NAV_BAR_HEIGHT = 36;

    EditorState::EditorState()
    {
        m_mainWindow = std::make_shared<UIWidget>("Crystal Tracer");
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

        m_toolBars = std::make_shared<ToolBarPanel>();
        m_toolBars->SetName("ToolBar");
        m_toolBars->SetPivot(Vector2f(0.f, .5f));
        m_toolBars->SetAnchorPoint(Vector2f(0.f, .5f));
        m_toolBars->SetSize(SizeLayout(0, .1f, 0, 1.f));
        m_toolBars->SetPosition(Vector2f(0, 0));
        layout->AppendElement(m_toolBars, Bound2f(Vector2f(0.f), Vector2f(0.08f, 1.f)));

        m_displayer = std::make_shared<DisplayPanel>();
        m_displayer->SetName("Display");
        m_displayer->SetPivot(Vector2f(0.f, .5f));
        m_displayer->SetAnchorPoint(Vector2f(0.f, .5f));
        m_displayer->SetSize(SizeLayout(0, .6f, 0, 1.f));
        m_displayer->SetPosition(Vector2f(0, 0));
        layout->AppendElement(m_displayer, Bound2f(Vector2f(.08f, 0.f), Vector2f(.75f, 1.f)));


        m_properties = std::make_shared<PropertiesPanel>();
        m_properties->SetName("Properties");
        m_properties->SetPivot(Vector2f(1.f, .5f));
        m_properties->SetAnchorPoint(Vector2f(1.f, .5f));
        m_properties->SetSize(SizeLayout(0, .3f, 0, 1.f));
        m_properties->SetPosition(Vector2f(0, 0));
        layout->AppendElement(m_properties, Bound2f(Vector2f(0.75f, 0.f), Vector2f(1.f, 1.f)));
    }

    EditorState::~EditorState()
    {}

    void EditorState::Update(const GameTimer& gameTimer)
    {
        UIState::Update(gameTimer);
    }
}
