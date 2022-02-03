#include "OpenGLFragmentShader.h"
namespace crystal
{
	OpenGLFragmentShader::OpenGLFragmentShader(GraphicsDevice* graphicsDevice, GLuint handle, const std::string& name)
		: m_pGraphicsDevice(graphicsDevice), m_bufferHandle(handle), m_shaderName(name)
	{
	}

	OpenGLFragmentShader::~OpenGLFragmentShader()
	{}
}