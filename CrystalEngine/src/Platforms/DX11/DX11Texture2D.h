#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11Texture2D
	{
	public:
		DX11Texture2D(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11Texture2D> tex2d, ComPtr<ID3D11ShaderResourceView> SRV);
		~DX11Texture2D();

	private:
		DX11GraphicsDevice*			m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11Texture2D>		m_texture2D = nullptr;
		ComPtr<ID3D11ShaderResourceView>	m_pSRV = nullptr;
	};
}