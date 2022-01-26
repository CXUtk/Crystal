#pragma once
#include <Core/Platform/Platforms.h>
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

		virtual void AppendOnResizeEvent(OnResizeEvent::Func eventHandler) override
		{
			_eventOnWindowResize += eventHandler;
		}

		virtual void AppendOnMouseScrollEvent(OnMouseScrollEvent::Func eventHandler) override
		{
			_eventOnMouseScroll += eventHandler;
		}

		virtual void AppendOnMouseButtonChangeEvent(OnMouseButtonChangeEvent::Func eventHandler) override
		{
			_eventOnMouseButtonChange += eventHandler;
		}

		virtual void AppendOnKeyChangeEvent(OnKeyChangeEvent::Func eventHandler) override
		{
			_eventOnKeyChange += eventHandler;
		}

	private:
		friend void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
		friend void glfw_mousescroll_callback_function(GLFWwindow* window, double xoffset, double yoffset);
		friend void glfw_key_callback_function(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void glfw_mousebutton_callback_function(GLFWwindow* window, int button, int action, int mods);

		GLFWwindow*		_window = nullptr;
		Vector2i		_windowSize{};

		OnResizeEvent				_eventOnWindowResize;
		OnKeyChangeEvent			_eventOnKeyChange;
		OnMouseScrollEvent			_eventOnMouseScroll;
		OnMouseButtonChangeEvent	_eventOnMouseButtonChange;


		void _resize(Vector2i newSize);
		void _mouseScroll(Vector2f offset);
		void _mouseButtonChange(int button, int action, int mods);
		void _keyChange(int key, int scancode, int action, int mods);
	};
}