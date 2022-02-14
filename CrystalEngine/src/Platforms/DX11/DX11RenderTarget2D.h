#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11RenderTarget2D : public IRenderTarget2D, public IShaderResource
	{
	public:
		DX11RenderTarget2D(DX11GraphicsDevice* graphicsDevice, const RenderTarget2DDescription& desc);
		~DX11RenderTarget2D() override;

		ID3D11DepthStencilView* GetDepthStencilView() const { return m_pDepthStencilView.Get(); }
		ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView.Get(); }
		virtual void GetShaderResourceHandle(void** pHandle) override;
	private:
		DX11GraphicsDevice*					m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11RenderTargetView>		m_pRenderTargetView = nullptr;
		ComPtr<ID3D11ShaderResourceView>	m_pShaderResourceView = nullptr;
		ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView = nullptr;
		D3D11_VIEWPORT						m_viewport{};

		friend DX11GraphicsDevice;
		void m_setToCurrentContext();
	};
}