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

		virtual CullingMode GetCullMode() const override;
		virtual FillMode GetFillMode() const override;

		virtual void SetCullMode(CullingMode mode) override;
		virtual void SetFillMode(FillMode mode) override;

		virtual void SetScissorState(bool enable) override;
		virtual void SetScissorRect(const Bound2i& rect) override;

		virtual void SetDepthTestState(bool enable) override;
		virtual void SetStencilTestState(bool enable) override;

		virtual void Apply() override;

		virtual void BindTexture(std::shared_ptr<Texture2D> texture, int index) override;
		virtual void BindSamplerState(std::shared_ptr<SamplerState> texture, int index) override;
	private:
		std::shared_ptr<VertexBuffer>	m_vertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer>	m_indexBuffer = nullptr;
		std::shared_ptr<ShaderProgram>	m_shaderProgram = nullptr;

		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;

		D3D11_RASTERIZER_DESC			m_rasterStateDesc{};
		ComPtr<ID3D11RasterizerState>	m_currentRasterizerState = nullptr;
		bool							m_needsRefreshRasterState = true;

		D3D11_DEPTH_STENCIL_DESC		m_depthStencilStateDesc{};
		ComPtr<ID3D11DepthStencilState>	m_currentDepthStencilState = nullptr;
		bool							m_needsRefreshDepthStencilState = true;

		static constexpr int NUM_TEXTURE_SLOTS = 8;
		ComPtr<ID3D11ShaderResourceView>	m_SRVSlots[NUM_TEXTURE_SLOTS];
		ComPtr<ID3D11SamplerState>			m_samplerStates[NUM_TEXTURE_SLOTS];

		bool		m_needsRefreshScissorRect = false;
		RECT		m_scissorRect{};
	};
}