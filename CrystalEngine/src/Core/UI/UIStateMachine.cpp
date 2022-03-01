#include "UIStateMachine.h"
#include "UIState.h"
#include <Engine.h>

#include <Core/Render/RenderExports.h>

namespace crystal
{
	UIStateMachine::UIStateMachine()
	{}
	UIStateMachine::~UIStateMachine()
	{}
    void UIStateMachine::Update(const GameTimer& gameTimer)
    {
        if (m_pNextUIState)
        {
            m_pCurrentUIState = m_pNextUIState;
            m_pNextUIState = nullptr;
        }
        if (!m_pCurrentUIState) return;
        m_pCurrentUIState->Update(gameTimer);
    }
    void UIStateMachine::Draw(const GameTimer & gameTimer)
    {
        if (!m_pCurrentUIState) return;
        auto engine = Engine::GetInstance();
        auto spriteBatch = engine->GetSpriteBatch();
        auto geometryRenderer = engine->GetGeometryRenderer();

        RenderPayload payload = {};
        payload.SpriteBatch = spriteBatch;
        payload.GeometryRenderer = geometryRenderer;

        m_pCurrentUIState->Draw(payload, gameTimer);
    }
    void UIStateMachine::AddState(const std::string& name, std::shared_ptr<UIState> state)
    {
        auto p = m_UIStates.find(name);
        if (p == m_UIStates.end())
        {
            m_UIStates[name] = state;
        }
        else
        {
            throw std::invalid_argument("Name already exist");
        }
    }
    void UIStateMachine::SetInitState(const std::string & name)
    {
        m_pCurrentUIState = m_UIStates[name];

        for (auto& pair : m_UIStates)
        {
            pair.second->Preprocess();
        }
    }
}