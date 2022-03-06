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
        void SetInitState(const std::string& name);

	private:
        std::map<std::string, std::shared_ptr<UIState>>	m_UIStates{};
        std::shared_ptr<UIState>                        m_pCurrentUIState = nullptr;
        std::shared_ptr<UIState>                        m_pNextUIState = nullptr;
        std::shared_ptr<IPipelineStateObject>           m_PSO = nullptr;
	};
}