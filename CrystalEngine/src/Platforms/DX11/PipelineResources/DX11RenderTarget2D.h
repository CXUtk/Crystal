#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11RenderTarget2D : public IRenderTarget2D
	{
	public:
		DX11RenderTarget2D(DX11GraphicsDevice* graphicsDevice, const RenderTarget2DDescription& desc);
		DX11RenderTarget2D(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11RenderTargetView> renderTargetView,
			ComPtr<ID3D11ShaderResourceView> shaderResourceView, ComPtr<ID3D11DepthStencilView> depthStencilView,
			const D3D11_VIEWPORT& viewPort);
		~DX11RenderTarget2D() override;

		virtual void GetShaderResourceHandle(void** pHandle) const override;
		virtual Vector2i GetSize() const override;

		void SetToCurrentContext(DX11GraphicsContext* context);
		void SetViewportToCurrentContext(DX11GraphicsContext* context);

		void ClearContent(ID3D11DeviceContext* context, ClearOptions options, 
			const Color4f& color, float depth, int stencil);
	private:
		DX11GraphicsDevice*					m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11RenderTargetView>		m_pRenderTargetView = nullptr;
		ComPtr<ID3D11ShaderResourceView>	m_pShaderResourceView = nullptr;
		ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView = nullptr;
		D3D11_VIEWPORT						m_viewport{};
	};
}