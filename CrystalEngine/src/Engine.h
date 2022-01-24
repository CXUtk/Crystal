#pragma once
#include "Crystal.h"
#include <functional>

namespace crystal
{
	using OnInitEventHandler = std::function<void()>;
	using OnUpdateEventHandler = std::function<void()>;
	using OnDrawEventHandler = std::function<void()>;
	using OnExitEventHandler = std::function<void()>;

	class Engine
	{
	public:
		~Engine();

		/**
		 * @brief Get instance of current Engine singleton
		 * @return pointer to Engine
		*/
		static Engine* GetInstance() { 
			static Engine engine;
			return &engine; 
		}

		void SetOnInitialize(OnInitEventHandler onInit) { _onInit = onInit; }
		void SetOnUpdate(OnUpdateEventHandler onUpdate) { _onUpdate = onUpdate; }
		void SetOnDraw(OnDrawEventHandler onDraw) { _onDraw = onDraw; }
		void SetOnExit(OnExitEventHandler onExit) { _onExit = onExit; }

	private:
		Engine();

		OnInitEventHandler _onInit;
		OnUpdateEventHandler _onUpdate;
		OnDrawEventHandler _onDraw;
		OnExitEventHandler _onExit;
	};
}