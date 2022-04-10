#include "OpenGLVertexShader.h"
namespace crystal
{
	OpenGLVertexShader::OpenGLVertexShader(GraphicsDevice* graphicsDevice, GLuint handle, const std::string& name)
		: m_pGraphicsDevice(graphicsDevice), m_bufferHandle(handle), m_shaderName(name)
	{
	}

	OpenGLVertexShader::~OpenGLVertexShader()
	{}
}