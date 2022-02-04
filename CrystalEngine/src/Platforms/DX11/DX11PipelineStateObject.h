#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11PipelineStateObject : public IPipelineStateObject
	{
	public:
		DX11PipelineStateObject(DX11GraphicsDevice* graphicsDevice);
		~DX11PipelineStateObject() override;

		virtual void BindVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
		virtual void BindIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;
		virtual void BindShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram) override;

		VertexBuffer* GetVertexBuffer() const { return ptr(m_vertexBuffer); }
		IndexBuffer* GetIndexBuffer() const { return ptr(m_indexBuffer); }
		ShaderProgram* GetShaderProgram() const { return ptr(m_shaderProgram); }
	private:
		std::shared_ptr<VertexBuffer>	m_vertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer>	m_indexBuffer = nullptr;
		std::shared_ptr<ShaderProgram>	m_shaderProgram = nullptr;

		DX11GraphicsDevice*			m_pGraphicsDevice = nullptr;
	};
}