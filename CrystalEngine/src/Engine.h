#pragma once
#include "Crystal.h"
#include "Core/Utils/GameTimer.h"
#include <functional>

namespace crystal
{
	//using OnInitEventHandler = std::function<void()>;
	//using OnUpdateEventHandler = std::function<void()>;
	//using OnDrawEventHandler = std::function<void()>;
	//using OnExitEventHandler = std::function<void()>;

	class Engine final
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

		/**
		 * @brief Get instance of current running window
		 * @return 
		*/
		IGameWindow* GetWindow() const;

		/**
		 * @brief GEt instance of engine input controller
		 * @return 
		*/
		InputController* GetInputController() const;

		IGraphicsDevice* GetGraphicsDevice() const;

		/**
		 * @brief Take control on the application and start the main game loop
		 * @param application 
		*/
		void Start(std::unique_ptr<Application>&& application);

	private:
		std::unique_ptr<Application>			m_application;
		std::unique_ptr<IPlatformProvider>		m_platformProvider;
		std::unique_ptr<InputController>		m_inputController;
		std::unique_ptr<SpriteBatch>			m_spriteBatch;

		double		m_fpsCap;			// Maximum FPS
		GameTimer	m_gameTimer;		// Game timer
	
		Engine();
	};
}