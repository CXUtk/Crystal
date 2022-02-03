#pragma once
#include "OpenGLCommon.h"

namespace crystal
{
	class OpenGLPipelineStateObject : public IPipelineStateObject
	{
	public:
		OpenGLPipelineStateObject(GraphicsDevice* graphicsDevice, GLuint handle);
		~OpenGLPipelineStateObject() override;

		virtual void BindVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
		virtual void BindIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;
		virtual void BindShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram) override;

		GLuint GetHandle() const { return m_VAOHandle; }
		VertexBuffer* GetVertexBuffer() const { return ptr(m_vertexBuffer); }
		IndexBuffer* GetIndexBuffer() const { return ptr(m_indexBuffer); }
		ShaderProgram* GetShaderProgram() const { return ptr(m_shaderProgram); }
	private:
		GraphicsDevice*		m_pGraphicsDevice = nullptr;
		GLuint				m_VAOHandle = 0;

		std::shared_ptr<VertexBuffer>	m_vertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer>	m_indexBuffer = nullptr;
		std::shared_ptr<ShaderProgram>	m_shaderProgram = nullptr;
	};
}