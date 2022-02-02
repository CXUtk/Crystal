#include "OpenGLGraphicsDevice.h"

namespace crystal
{
	OpenGLGraphicsDevice::OpenGLGraphicsDevice(const InitArgs& args, GLFWGameWindow* window)
		: m_pWindow(window)
	{
		// Initialize GLAD and configs
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::exception("Failed to load glad!");
		}

		window->AppendOnResizeEvent([](Vector2i size) {
			glViewport(0, 0, size.x, size.y);
		});
	}
	
	OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
	{}

	void OpenGLGraphicsDevice::Clear(ClearOptions options, const Color4f & color, float depth, int stencil)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		int flags = 0;
		if (options & ClearOptions::Target)
		{
			flags |= GL_COLOR_BUFFER_BIT;
		}
		if (options & ClearOptions::Depth)
		{
			flags |= GL_DEPTH_BUFFER_BIT;
		}
		if (options & ClearOptions::Stencil)
		{
			flags |= GL_STENCIL_BUFFER_BIT;
		}
		glClear(flags);
	}

	void OpenGLGraphicsDevice::Present()
	{
		glfwSwapBuffers(m_pWindow->GetGLFWWindow());
	}

	void OpenGLGraphicsDevice::DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices)
	{
		glDrawArrays(PrimitiveTypeToTopologyConvert(primitiveType), offset, numVertices);
	}

	void OpenGLGraphicsDevice::DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices, size_t indexOffset, size_t vertexOffset)
	{
		glDrawElements(PrimitiveTypeToTopologyConvert(primitiveType), numIndices, , (void*)vertexOffset);
	}

	std::shared_ptr<IVertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(const VertexBufferDescription & desc, void* src, size_t size)
	{
		return std::shared_ptr<IVertexBuffer>();
	}

	std::shared_ptr<IIndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(const IndexBufferDescription& desc, void* src, size_t size)
	{
		return std::shared_ptr<IIndexBuffer>();
	}

	std::shared_ptr<IVertexShader> OpenGLGraphicsDevice::CreateVertexShaderFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint)
	{
		return std::shared_ptr<IVertexShader>();
	}

	std::shared_ptr<IFragmentShader> OpenGLGraphicsDevice::CreateFragmentShaderFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint)
	{
		return std::shared_ptr<IFragmentShader>();
	}

	std::shared_ptr<IShaderProgram> OpenGLGraphicsDevice::CreateShaderProgramFromFile(const std::string& path)
	{
		return std::shared_ptr<IShaderProgram>();
	}
}
