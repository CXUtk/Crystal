#include "GLFWGameWindow.h"
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/InitArgs.h>
#include <Core/Utils/Logger.h>

namespace crystal
{
	constexpr size_t MAX_KEYS = 512;
	template<size_t N>
	constexpr std::array<KeyCode, N> generateKeyCodeMapper()
	{
		std::array<KeyCode, N> M{};
		M[GLFW_KEY_W] = KeyCode::CRYSTAL_W_KEY;
		M[GLFW_KEY_A] = KeyCode::CRYSTAL_A_KEY;
		M[GLFW_KEY_S] = KeyCode::CRYSTAL_S_KEY;
		M[GLFW_KEY_D] = KeyCode::CRYSTAL_D_KEY;
		M[GLFW_KEY_M] = KeyCode::CRYSTAL_M_KEY;

		M[GLFW_KEY_UP] = KeyCode::CRYSTAL_UP_KEY;
		M[GLFW_KEY_DOWN] = KeyCode::CRYSTAL_DOWN_KEY;
		M[GLFW_KEY_LEFT] = KeyCode::CRYSTAL_LEFT_KEY;
		M[GLFW_KEY_RIGHT] = KeyCode::CRYSTAL_RIGHT_KEY;

		M[GLFW_KEY_ENTER] = KeyCode::CRYSTAL_ENTER_KEY;
		M[GLFW_KEY_LEFT_CONTROL] = KeyCode::CRYSTAL_LEFT_CTRL_KEY;
		M[GLFW_KEY_RIGHT_CONTROL] = KeyCode::CRYSTAL_RIGHT_CTRL_KEY;
		M[GLFW_KEY_LEFT_ALT] = KeyCode::CRYSTAL_LEFT_ALT_KEY;
		M[GLFW_KEY_RIGHT_ALT] = KeyCode::CRYSTAL_RIGHT_ALT_KEY;


		M[GLFW_KEY_1] = KeyCode::CRYSTAL_1_KEY;
		M[GLFW_KEY_2] = KeyCode::CRYSTAL_2_KEY;
		M[GLFW_KEY_3] = KeyCode::CRYSTAL_3_KEY;
		M[GLFW_KEY_4] = KeyCode::CRYSTAL_4_KEY;
		M[GLFW_KEY_5] = KeyCode::CRYSTAL_5_KEY;
		M[GLFW_KEY_6] = KeyCode::CRYSTAL_6_KEY;
		M[GLFW_KEY_7] = KeyCode::CRYSTAL_7_KEY;
		M[GLFW_KEY_8] = KeyCode::CRYSTAL_8_KEY;
		M[GLFW_KEY_9] = KeyCode::CRYSTAL_9_KEY;
		M[GLFW_KEY_0] = KeyCode::CRYSTAL_0_KEY;


		M[GLFW_KEY_EQUAL] = KeyCode::CRYSTAL_PLUS_KEY;
		M[GLFW_KEY_MINUS] = KeyCode::CRYSTAL_MINUS_KEY;
		M[GLFW_KEY_GRAVE_ACCENT] = KeyCode::CRYSTAL_TILDE_KEY;

		M[GLFW_KEY_SPACE] = KeyCode::CRYSTAL_SPACE_KEY;
		M[GLFW_KEY_ESCAPE] = KeyCode::CRYSTAL_ESC_KEY;

		return M;
	}

	template<size_t T>
	constexpr std::array<MouseButtonCode, T> generateMouseButtonCodeMapper()
	{
		std::array<MouseButtonCode, T> M{ };

		M[GLFW_MOUSE_BUTTON_LEFT] = MouseButtonCode::LEFT_BUTTON;
		M[GLFW_MOUSE_BUTTON_RIGHT] = MouseButtonCode::RIGHT_BUTTON;
		M[GLFW_MOUSE_BUTTON_MIDDLE] = MouseButtonCode::MIDDLE_BUTTON;
		return M;
	}

	template<size_t T>
	constexpr std::array<InputAction, T> generateInputActionMap()
	{
		std::array<InputAction, T> M{ };

		M[GLFW_RELEASE] = InputAction::RELEASE;
		M[GLFW_PRESS] = InputAction::PRESS;
		M[GLFW_REPEAT] = InputAction::REPEAT;
		return M;
	}

	constexpr auto keyCodeMap = generateKeyCodeMapper<MAX_KEYS>();
	constexpr auto inputActionMap = generateInputActionMap<(size_t)InputAction::__COUNT>();
	constexpr auto mouseButtonCodeMap = generateMouseButtonCodeMapper<(size_t)MouseButtonCode::__COUNT>();

	void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		auto gameWindow = (GLFWGameWindow*)glfwGetWindowUserPointer(window);
		gameWindow->_resize(glm::ivec2(width, height));
	}

	void glfw_mousescroll_callback_function(GLFWwindow* window, double xoffset, double yoffset)
	{
		auto gameWindow = (GLFWGameWindow*)glfwGetWindowUserPointer(window);
		gameWindow->_mouseScroll(glm::vec2(xoffset, yoffset));
	}

	void glfw_key_callback_function(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto gameWindow = (GLFWGameWindow*)glfwGetWindowUserPointer(window);
		gameWindow->_keyChange(key, scancode, action, mods);
		//auto inputController = Engine::GetInstance()->GetInputController();
		//if (key < 0 || key >= MAX_KEYS) return;
		//inputController->TriggerKeyChange(keyCodeMap[key], action != GLFW_RELEASE);
	}

	void glfw_mousebutton_callback_function(GLFWwindow* window, int button, int action, int mods)
	{
		auto gameWindow = (GLFWGameWindow*)glfwGetWindowUserPointer(window);
		gameWindow->_mouseButtonChange(button, action, mods);
	}


	GLFWGameWindow::GLFWGameWindow(const InitArgs& args)
	{
		m_windowSize = Vector2i(args.WindowWidth, args.WindowHeight);

		m_window = glfwCreateWindow(m_windowSize.x, m_windowSize.y,
			args.WindowTitle, nullptr, nullptr);
		if (!m_window)
		{
			throw std::exception("Failed to create window");
		}

		GlobalLogger::Log(SeverityLevel::Debug, "GLFWWindow construct");

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);

		glfwSetFramebufferSizeCallback(m_window, glfw_framebuffer_size_callback);
		glfwSetScrollCallback(m_window, glfw_mousescroll_callback_function);
		glfwSetMouseButtonCallback(m_window, glfw_mousebutton_callback_function);
		glfwSetKeyCallback(m_window, glfw_key_callback_function);
	}
	GLFWGameWindow::~GLFWGameWindow()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
		}
		m_window = nullptr;
		GlobalLogger::Log(SeverityLevel::Debug, "GLFWWindow destruct");
	}

	void GLFWGameWindow::BeginFrame()
	{
	}

	void GLFWGameWindow::EndFrame()
	{
	}

	bool GLFWGameWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

	bool GLFWGameWindow::IsPaused() const
	{
		return false;
	}

	void GLFWGameWindow::PollEvents()
	{
		glfwPollEvents();
	}

	Vector2i GLFWGameWindow::GetMousePos() const
	{
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);

		return Vector2i(x, m_windowSize.y - y - 1);
	}

	void GLFWGameWindow::AppendOnResizeEvent(OnResizeEvent::Func eventHandler)
	{
		m_eventOnWindowResize += eventHandler;
	}

	void GLFWGameWindow::AppendOnMouseScrollEvent(OnMouseScrollEvent::Func eventHandler)
	{
		m_eventOnMouseScroll += eventHandler;
	}

	void GLFWGameWindow::AppendOnMouseButtonChangeEvent(OnMouseButtonChangeEvent::Func eventHandler)
	{
		m_eventOnMouseButtonChange += eventHandler;
	}

	void GLFWGameWindow::AppendOnKeyChangeEvent(OnKeyChangeEvent::Func eventHandler)
	{
		m_eventOnKeyChange += eventHandler;
	}

	void GLFWGameWindow::_resize(Vector2i newSize)
	{
		m_windowSize = newSize;
		m_eventOnWindowResize.Invoke(newSize);
	}

	void GLFWGameWindow::_mouseScroll(Vector2f offset)
	{
		m_eventOnMouseScroll.Invoke(offset);
	}

	void GLFWGameWindow::_mouseButtonChange(int button, int action, int mods)
	{
		MouseButtonEventArgs args{};
		args.ButtonCode = mouseButtonCodeMap[button];
		args.Action = inputActionMap[action];
		m_eventOnMouseButtonChange.Invoke(args);
	}

	void GLFWGameWindow::_keyChange(int key, int scancode, int action, int mods)
	{
		KeyEventArgs args{};
		// Unknown key
		if (key == -1) return;
		args.KeyCode = keyCodeMap[key];
		args.Action = inputActionMap[action];
		args.Mods = (KeyMODState)mods;
		m_eventOnKeyChange.Invoke(args);
	}

}