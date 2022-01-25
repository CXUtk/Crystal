#pragma once
#include "Crystal.h"
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
		 * @brief Take control on the application and start the main game loop
		 * @param application 
		*/
		void Start(std::unique_ptr<Application>&& application);

	private:
		std::unique_ptr<Application> _application;
	
		Engine();
	};
}