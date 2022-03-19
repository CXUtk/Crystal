#pragma once
#include <CrystalEngine/src/Core/UI/UIExports.h>

#include "View/DisplayPanel.h"
#include "View/PropertiesPanel.h"
#include "View/ToolBarPanel.h"

namespace tracer
{
    class EditorState : public crystal::UIState
    {
    public:
        EditorState();
        virtual ~EditorState() override;

        virtual void Update(const crystal::GameTimer& gameTimer) override;

    private:
        std::shared_ptr<crystal::UIWidget>       m_mainWindow = nullptr;
        std::shared_ptr<crystal::UITextButton>   m_startRenderButton = nullptr;

        std::shared_ptr<crystal::UIElement>         m_body = nullptr;
        std::shared_ptr<crystal::UIElement>         m_navigator = nullptr;

        std::shared_ptr<ToolBarPanel>               m_toolBars = nullptr;
        std::shared_ptr<DisplayPanel>               m_displayer = nullptr;
        std::shared_ptr<PropertiesPanel>            m_properties = nullptr;
    };
}