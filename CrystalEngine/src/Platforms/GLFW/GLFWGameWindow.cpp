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

		glViewport(0, 0, width, height);
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
		_windowSize = Vector2i(args.WindowWidth, args.WindowHeight);

		_window = glfwCreateWindow(_windowSize.x, _windowSize.y,
			args.WindowTitle, nullptr, nullptr);
		if (!_window)
		{
			throw std::exception("Failed to create window");
		}

		GlobalLogger::Log(SeverityLevel::Debug, "GLFWWindow construct");

		glfwMakeContextCurrent(_window);
		glfwSetWindowUserPointer(_window, this);

		glfwSetFramebufferSizeCallback(_window, glfw_framebuffer_size_callback);
		glfwSetScrollCallback(_window, glfw_mousescroll_callback_function);
		glfwSetMouseButtonCallback(_window, glfw_mousebutton_callback_function);
		glfwSetKeyCallback(_window, glfw_key_callback_function);
	}
	GLFWGameWindow::~GLFWGameWindow()
	{
		if (_window)
		{
			glfwDestroyWindow(_window);
		}
		_window = nullptr;
		GlobalLogger::Log(SeverityLevel::Debug, "GLFWWindow destruct");
	}

	void GLFWGameWindow::BeginFrame()
	{}

	void GLFWGameWindow::EndFrame()
	{
		glfwSwapBuffers(_window);
	}

	bool GLFWGameWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(_window);
	}

	void GLFWGameWindow::PollEvents()
	{
		glfwPollEvents();
	}

	Vector2i GLFWGameWindow::GetMousePos() const
	{
		double x, y;
		glfwGetCursorPos(_window, &x, &y);

		return Vector2i(x, _windowSize.y - y - 1);
	}

	void GLFWGameWindow::_resize(Vector2i newSize)
	{
		_windowSize = newSize;
		_eventOnWindowResize.Invoke(newSize);
	}

	void GLFWGameWindow::_mouseScroll(Vector2f offset)
	{
		_eventOnMouseScroll.Invoke(offset);
	}

	void GLFWGameWindow::_mouseButtonChange(int button, int action, int mods)
	{
		MouseButtonEventArgs args{};
		args.ButtonCode = mouseButtonCodeMap[button];
		args.Action = inputActionMap[action];
		_eventOnMouseButtonChange.Invoke(args);
	}

	void GLFWGameWindow::_keyChange(int key, int scancode, int action, int mods)
	{
		KeyEventArgs args{};
		// Unknown key
		if (key == -1) return;
		args.KeyCode = keyCodeMap[key];
		args.Action = inputActionMap[action];
		args.mods = mods;
		_eventOnKeyChange.Invoke(args);
	}

}