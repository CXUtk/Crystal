#include "OpenGLProvider.h"
#include "OpenGLGraphicsDevice.h"
#include <Platforms/GLFW/GLFWGameWindow.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/InitArgs.h>
#include <Core/Utils/Logger.h>
#include <Core/Utils/Misc.h>

namespace crystal
{
    void APIENTRY glDebugOutput(GLenum source,
                                   GLenum type,
                                   unsigned int id,
                                   GLenum severity,
                                   GLsizei length,
                                   const char* message,
                                   const void* userParam);

    OpenGLProvider::OpenGLProvider(const InitArgs& args)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, args.WindowResizable ? GLFW_TRUE : GLFW_FALSE);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
		GlobalLogger::Log(SeverityLevel::Debug, "GLFW construct");
        InitOpenGLCommons();

		m_gameWindow = std::make_unique<GLFWGameWindow>(args);
        m_graphicsDevice = std::make_unique<OpenGLGraphicsDevice>(args, ptr(m_gameWindow));

        mountDebugErrorLog();
	}


    OpenGLProvider::~OpenGLProvider()
	{
		m_gameWindow.reset();
		glfwTerminate();
		GlobalLogger::Log(SeverityLevel::Debug, "GLFW destruct");
	}
	IGameWindow* OpenGLProvider::GetGameWindow() const
	{
		return ptr(m_gameWindow);
	}
	IFileSystem* OpenGLProvider::GetFileSystem() const
	{
		return nullptr;
	}

    IGraphicsDevice* OpenGLProvider::GetGraphicsDevice() const
    {
        return ptr(m_graphicsDevice);
    }

    void OpenGLProvider::mountDebugErrorLog()
    {
#ifdef _DEBUG
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            GlobalLogger::Log(SeverityLevel::Info, "Successfully enabled debug function");
        }
        else
        {
            GlobalLogger::Log(SeverityLevel::Warning, "Cannot enable debug function");
        }
#endif
    }


    void APIENTRY glDebugOutput(GLenum source,
                                GLenum type,
                                unsigned int id,
                                GLenum severity,
                                GLsizei length,
                                const char* message,
                                const void* userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        SeverityLevel level = SeverityLevel::Info;

        std::string str;
        str.append("---------------\n");
        str.append(string_format("Debug message (%d): %s\n", id, message));

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             str.append("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   str.append("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: str.append("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     str.append("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     str.append("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           str.append("Source: Other"); break;
        }

        str.append("\n");

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
        {
            level = SeverityLevel::Error;
            str.append("Type: Error");
            break;
        }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        {
            level = SeverityLevel::Warning;
            str.append("Type: Deprecated Behaviour");
            break;
        }
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        {
            level = SeverityLevel::Warning;
            str.append("Type: Undefined Behaviour");
            break;
        }
        case GL_DEBUG_TYPE_PORTABILITY:         str.append("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         str.append("Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              str.append("Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          str.append("Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           str.append("Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               str.append("Type: Other"); break;
        }

        str.append("\n");

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         str.append("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       str.append("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          str.append("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: str.append("Severity: notification"); break;
        }

        str.append("\n");

        GlobalLogger::Log(level, "%s", str.c_str());

        if (level == SeverityLevel::Error)
        {
            throw std::exception("OpenGL error triggered!");
        }
    }
}