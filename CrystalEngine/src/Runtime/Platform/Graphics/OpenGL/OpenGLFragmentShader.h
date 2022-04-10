#pragma once
#include "OpenGLCommon.h"

namespace crystal
{
	class OpenGLFragmentShader : public IFragmentShader
	{
	public:
		OpenGLFragmentShader(GraphicsDevice* graphicsDevice, GLuint handle, const std::string& name);
		~OpenGLFragmentShader() override;

		GLuint GetHandle() const { return m_bufferHandle; }
		std::string GetName() const { return m_shaderName; }
	private:
		GraphicsDevice*		m_pGraphicsDevice = nullptr;
		GLuint				m_bufferHandle{};
		std::string			m_shaderName{};
	};

}