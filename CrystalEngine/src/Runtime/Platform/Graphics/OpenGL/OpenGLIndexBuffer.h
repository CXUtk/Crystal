#pragma once
#include "OpenGLCommon.h"

namespace crystal
{
	class OpenGLIndexBuffer : public IIndexBuffer
	{
	public:
		OpenGLIndexBuffer(GraphicsDevice* graphicsDevice, GLuint handle, const IndexBufferDescription& desc);
		~OpenGLIndexBuffer() override;

		GLuint GetHandle() const { return m_bufferHandle; }
		GLenum GetDataType() const { return m_indexBufferType; }
	private:
		GraphicsDevice*		m_pGraphicsDevice = nullptr;
		GLuint				m_bufferHandle{};
		GLenum				m_indexBufferType{};
	};

}