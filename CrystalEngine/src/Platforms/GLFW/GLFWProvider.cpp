#include "GLFWProvider.h"
#include "GLFWGameWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/InitArgs.h>

namespace crystal
{
	GLFWProvider::GLFWProvider(const InitArgs& args)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, args.WindowResizable ? GLFW_TRUE : GLFW_FALSE);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
		_gameWindow = std::make_unique<GLFWGameWindow>(args);
		_fileSystem = std::make_unique<WindowsFileSystem>();
	}
	GLFWProvider::~GLFWProvider()
	{
	}
}