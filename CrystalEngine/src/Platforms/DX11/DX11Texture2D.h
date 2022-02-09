#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11Texture2D : public IDX11ShaderResource
	{
	public:
		DX11Texture2D(DX11GraphicsDevice* graphicsDevice, const std::string& path, const Texture2DDescription& texDesc);
		DX11Texture2D(DX11GraphicsDevice* graphicsDevice, const uint8_t* src, size_t size, const Texture2DDescription& texDesc);
		~DX11Texture2D() override;

		ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const override { return m_pSRV; }
	private:
		DX11GraphicsDevice*					m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11Texture2D>				m_texture2D = nullptr;
		ComPtr<ID3D11ShaderResourceView>	m_pSRV = nullptr;
	};
}