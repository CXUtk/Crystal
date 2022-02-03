#include "OpenGLIndexBuffer.h"
namespace crystal
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(GraphicsDevice* graphicsDevice, GLuint handle,
		const IndexBufferDescription& desc)
		: m_pGraphicsDevice(graphicsDevice), m_bufferHandle(handle)
	{
		m_indexBufferType = DataFormatConvert(desc.Format);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{}
}