#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11RenderTarget2D : public IDX11ShaderResource
	{
	public:
		DX11RenderTarget2D(DX11GraphicsDevice* graphicsDevice, const RenderTarget2DDescription& desc);
		~DX11RenderTarget2D() override;

		void SetToCurrent();
		ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const override { return m_pShaderResourceView; }
	private:
		DX11GraphicsDevice*			m_pGraphicsDevice = nullptr;

		ComPtr<ID3D11RenderTargetView>		m_pRenderTargetView = nullptr;
		ComPtr<ID3D11ShaderResourceView>	m_pShaderResourceView = nullptr;
		ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView = nullptr;
		D3D11_VIEWPORT						m_viewport{};
	};
}