#pragma once
#include "OpenGLCommon.h"

namespace crystal
{
	class OpenGLVertexBuffer : public IVertexBuffer
	{
	public:
		OpenGLVertexBuffer(GraphicsDevice* graphicsDevice, GLuint handle);
		~OpenGLVertexBuffer() override;

		void BindVertexLayout(const VertexLayout& layout) override;
		const VertexLayout& GetVertexLayout() const { return m_vertexLayout; }

		GLuint GetHandle() const { return m_bufferHandle; }
	private:
		GraphicsDevice*		m_pGraphicsDevice = nullptr;
		GLuint				m_bufferHandle{};
		VertexLayout		m_vertexLayout{};
	};

}