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

		virtual CullingMode GetCullMode() const override;
		virtual FillMode GetFillMode() const override;

		virtual void SetCullMode(CullingMode mode) override;
		virtual void SetFillMode(FillMode mode) override;

		virtual void SetScissorState(bool enable) override;
		virtual void SetScissorRect(const Bound2i& rect) override;

		virtual void SetDepthTestState(bool enable) override;
		virtual void SetStencilTestState(bool enable) override;

		//virtual void Begin() override;
		//virtual void End() override;

		virtual void BindShaderResource(std::shared_ptr<IShaderResource> shaderResource, int index) override;
		virtual void BindSamplerState(std::shared_ptr<SamplerState> texture, int index) override;

		virtual PipelineStateObjectDirtyFlags CheckDirtyFlag(DX11PipelineStateObject* other);
		virtual void Apply(PipelineStateObjectDirtyFlags dirtyFlags);
	private:
		std::shared_ptr<VertexBuffer>	m_vertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer>	m_indexBuffer = nullptr;
		//std::shared_ptr<ShaderProgram>	m_shaderProgram = nullptr;

		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;

		D3D11_RASTERIZER_DESC			m_rasterStateDesc{};
		ComPtr<ID3D11RasterizerState>	m_currentRasterizerState = nullptr;
		bool							m_needsRefreshRasterState = true;

		D3D11_DEPTH_STENCIL_DESC		m_depthStencilStateDesc{};
		ComPtr<ID3D11DepthStencilState>	m_currentDepthStencilState = nullptr;
		bool							m_needsRefreshDepthStencilState = true;

		static constexpr int MAX_SHADER_RESOURCES_COUNT = 8;
		ComPtr<ID3D11ShaderResourceView>	m_SRVSlots[MAX_SHADER_RESOURCES_COUNT]{};
		ComPtr<ID3D11SamplerState>			m_samplerStates[MAX_SHADER_RESOURCES_COUNT]{};

		bool		m_needsRefreshScissorRect = false;
		RECT		m_scissorRect{};


		bool m_checkRasterizerState(DX11PipelineStateObject* other);
		bool m_checkDepthStencilState(DX11PipelineStateObject* other);
	};
}