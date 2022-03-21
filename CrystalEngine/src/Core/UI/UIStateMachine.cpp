#include "UIStateMachine.h"
#include "UIState.h"
#include <Engine.h>

#include <Core/Render/RenderExports.h>
#include <Core/Asset/AssetManager.h>

namespace crystal
{
	UIStateMachine::UIStateMachine()
	{
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();
        auto assetManager = Engine::GetInstance()->GetAssetManager();

        m_PSO = graphicsDevice->CreatePipelineStateObject();

        m_PSO->SetBlendState(graphicsDevice->CreateBlendStateFromTemplate(BlendStates::AlphaBlend));
        m_PSO->SetDepthStencilState(graphicsDevice->CreateDepthStencilStateFromTemplate(DepthStencilStates::NoDepthTest));
        m_PSO->SetRasterState(graphicsDevice->CreateRasterStateFromTemplate(RasterStates::CullNone));

        m_whiteTexture = assetManager->LoadAsset<ITexture2D>("Crystal:white");
        m_frameTexture = assetManager->LoadAsset<ITexture2D>("package1:Frame");
        m_panelTexture = assetManager->LoadAsset<ITexture2D>("package1:Panel");
    }

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

        RenderPayload payload = {};
        payload.SpriteBatch = spriteBatch;
        payload.PSO = m_PSO;

        spriteBatch->Begin(SpriteSortMode::Deferred, m_PSO);
        {
            m_pCurrentUIState->Draw(payload, gameTimer);
        }
        spriteBatch->End();
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

    void UIStateMachine::SetState(const std::string& name)
    {
        auto& nextState = m_UIStates[name];
        if (m_pCurrentUIState != nextState)
        {
            if (m_pCurrentUIState)
            {
                m_pCurrentUIState->DeActivate();
            }
            m_pCurrentUIState = nextState;
            m_pCurrentUIState->Activate();
        }
    }
}