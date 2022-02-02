#pragma once
#include <Crystal.h>
#include <Core/Platform/Platforms.h>

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
		virtual bool IsPaused() const override;
		virtual void PollEvents() override;

		Vector2i GetMousePos() const override;
		Vector2i GetWindowSize() const override { return m_windowSize; }

		GLFWwindow* GetGLFWWindow() const { return m_window; }

		virtual void AppendOnResizeEvent(OnResizeEvent::Func eventHandler) override;

		virtual void AppendOnMouseScrollEvent(OnMouseScrollEvent::Func eventHandler) override;

		virtual void AppendOnMouseButtonChangeEvent(OnMouseButtonChangeEvent::Func eventHandler) override;

		virtual void AppendOnKeyChangeEvent(OnKeyChangeEvent::Func eventHandler) override;

	private:
		friend void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
		friend void glfw_mousescroll_callback_function(GLFWwindow* window, double xoffset, double yoffset);
		friend void glfw_key_callback_function(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void glfw_mousebutton_callback_function(GLFWwindow* window, int button, int action, int mods);

		GLFWwindow*		m_window = nullptr;
		Vector2i		m_windowSize{};
		bool			m_isPaused;

		OnResizeEvent				m_eventOnWindowResize;
		OnKeyChangeEvent			m_eventOnKeyChange;
		OnMouseScrollEvent			m_eventOnMouseScroll;
		OnMouseButtonChangeEvent	m_eventOnMouseButtonChange;


		void _resize(Vector2i newSize);
		void _mouseScroll(Vector2f offset);
		void _mouseButtonChange(int button, int action, int mods);
		void _keyChange(int key, int scancode, int action, int mods);
	};
}