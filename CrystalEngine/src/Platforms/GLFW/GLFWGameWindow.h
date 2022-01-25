#pragma once
#include <Platforms/Interfaces/IGameWindow.h>
#include <Crystal.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace crystal
{
	class GLFWGameWindow : public IGameWindow
	{
	public:
		GLFWGameWindow(const InitArgs& settings);
		~GLFWGameWindow();

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual bool ShouldClose() const override;
		virtual void PollEvents() override;


		Vector2i GetMousePos() const override;
		Vector2i GetWindowSize() const override { return _windowSize; }

		virtual void AppendOnResizeEvent(Event<Vector2i>::Func eventHandler) override
		{
			_eventOnWindowResize += eventHandler;
		}

		void Resize(Vector2i newSize)
		{
			_eventOnWindowResize.Invoke(newSize);
		}

	private:
		GLFWwindow* _window;
		Vector2i _windowSize;

		Event<Vector2i> _eventOnWindowResize;
	};
}