#include "OpenGLVertexBuffer.h"
namespace crystal
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(GraphicsDevice* graphicsDevice, GLuint handle)
		: m_pGraphicsDevice(graphicsDevice), m_bufferHandle(handle)
	{}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{}

	void OpenGLVertexBuffer::BindVertexLayout(const VertexLayout& layout)
	{
		m_vertexLayout = layout;
	}
}