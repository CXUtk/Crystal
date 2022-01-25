#pragma once
#include <Crystal.h>

namespace crystal
{
	class Application
	{
	public:
		Application() {}
		virtual ~Application() = 0;

		/**
		 * @brief Set engine pointer
		*/
		virtual void SetEngine(Engine* engine) final { _engine = engine; }


		/**
		* @brief Initialization of the application after the engine is ready
		* @param engine The instance of game engine
		*/
		virtual void Initialize() = 0;

		/**
		 * @brief Specify how to update each frame, execute once per frame
		 * @param deltaTime How much time was elapsed during the last frame
		*/
		virtual void Update(double deltaTime) = 0;


		/**
		 * @brief Specify how to draw each frame, execute once per frame, after each Update
		 * @param deltaTime How much time was elapsed during the last frame
		*/
		virtual void Draw(double deltaTime) = 0;

	protected:
		Engine* _engine = nullptr;
	};
}