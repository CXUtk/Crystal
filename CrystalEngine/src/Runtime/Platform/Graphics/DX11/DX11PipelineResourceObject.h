#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11PipelineResourceObject : public IPipelineResourceObject
	{
	public:
		DX11PipelineResourceObject(DX11GraphicsDevice* graphicsDevice, DX11GraphicsContext* graphicsContext);
		~DX11PipelineResourceObject() override;

		virtual void SetShaderResource(std::shared_ptr<IShaderResource> shaderResource, int slot) override;
		virtual void SetSamplerState(std::shared_ptr<ISamplerState> samplerState, int slot) override;
		virtual void SetShaderProgram(std::shared_ptr<IShaderProgram> shaderProgram) override;

		virtual void SetVertexBuffer(std::shared_ptr<IVertexBuffer> vertexBuffer) override;
		virtual void SetIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer) override;

		void Load();
		void Unload();
	private:
		static constexpr int MAX_SHADER_RESOURCES_SLOTS = 8;

		DX11GraphicsDevice*		m_pGraphicsDevice = nullptr;
		DX11GraphicsContext*	m_pGraphicsContext = nullptr;

		std::shared_ptr<DX11VertexBuffer>		m_pVertexBuffer = nullptr;
		std::shared_ptr<DX11IndexBuffer>		m_pIndexBuffer = nullptr;
		std::shared_ptr<DX11ShaderProgram>		m_pShaderProgram = nullptr;

		std::shared_ptr<IShaderResource>		m_SRVSlots[MAX_SHADER_RESOURCES_SLOTS]{};
		std::shared_ptr<DX11SamplerState>		m_SamplerSlots[MAX_SHADER_RESOURCES_SLOTS]{};
	};
}