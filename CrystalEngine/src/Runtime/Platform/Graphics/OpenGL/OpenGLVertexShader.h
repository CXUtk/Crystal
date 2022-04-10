#pragma once
#include "OpenGLCommon.h"

namespace crystal
{
	class OpenGLVertexShader : public IVertexShader
	{
	public:
		OpenGLVertexShader(GraphicsDevice* graphicsDevice, GLuint handle, const std::string& name);
		~OpenGLVertexShader() override;

		GLuint GetHandle() const { return m_bufferHandle; }
		std::string GetName() const { return m_shaderName; }
	private:
		GraphicsDevice*		m_pGraphicsDevice = nullptr;
		GLuint				m_bufferHandle{};
		std::string			m_shaderName{};
	};

}