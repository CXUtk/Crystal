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

		virtual void Apply();
	private:
		std::shared_ptr<VertexBuffer>	m_vertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer>	m_indexBuffer = nullptr;
		//std::shared_ptr<ShaderProgram>	m_shaderProgram = nullptr;

		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;

		D3D11_RASTERIZER_DESC			m_rasterStateDesc{};
		ComPtr<ID3D11RasterizerState>	m_currentRasterizerState = nullptr;
		bool							m_needsRefreshRasterState = false;

		D3D11_DEPTH_STENCIL_DESC		m_depthStencilStateDesc{};
		ComPtr<ID3D11DepthStencilState>	m_currentDepthStencilState = nullptr;
		bool							m_needsRefreshDepthStencilState = false;

		std::shared_ptr<BlendState>		m_blendState = nullptr;

		bool		m_needsRefreshScissorRect = false;
		RECT		m_scissorRect{};


		bool m_checkRasterizerState(DX11PipelineStateObject* other);
		bool m_checkDepthStencilState(DX11PipelineStateObject* other);
	};
}