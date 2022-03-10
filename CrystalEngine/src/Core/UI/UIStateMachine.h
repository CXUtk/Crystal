#pragma once
#include "Common/UICommon.h"
namespace crystal
{
	class UIStateMachine
	{
	public:
		UIStateMachine();
		~UIStateMachine();

		void Update(const GameTimer& gameTimer);
		void Draw(const GameTimer& gameTimer);

        void AddState(const std::string& name, std::shared_ptr<UIState> state);
        void SetState(const std::string& name);

        std::shared_ptr<ITexture2D> GetWhiteTexture() { return m_whiteTexture; }
        std::shared_ptr<ITexture2D> GetFrameTexture() { return m_frameTexture; }
        std::shared_ptr<ITexture2D> GetPanelTexture() { return m_panelTexture; }
	private:
        std::map<std::string, std::shared_ptr<UIState>>	m_UIStates{};
        std::shared_ptr<UIState>                        m_pCurrentUIState = nullptr;
        std::shared_ptr<UIState>                        m_pNextUIState = nullptr;
        std::shared_ptr<IPipelineStateObject>           m_PSO = nullptr;

        std::shared_ptr<ITexture2D>                     m_whiteTexture = nullptr;
        std::shared_ptr<ITexture2D>                     m_frameTexture = nullptr;
        std::shared_ptr<ITexture2D>                     m_panelTexture = nullptr;
	};
}