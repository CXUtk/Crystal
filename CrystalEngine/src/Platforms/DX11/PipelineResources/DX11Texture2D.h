#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11Texture2D : public ITexture2D
	{
	public:
		DX11Texture2D(DX11GraphicsDevice* graphicsDevice, const std::string& path, const Texture2DDescription& texDesc);
		DX11Texture2D(DX11GraphicsDevice* graphicsDevice, const uint8_t* src, size_t size, const Texture2DDescription& texDesc);
		~DX11Texture2D() override;

		virtual Vector2i GetSize() const override { return m_size; }
		virtual void GetShaderResourceHandle(void* pHandle) const override;
	private:
		DX11GraphicsDevice*					m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11Texture2D>				m_texture2D = nullptr;
		ComPtr<ID3D11ShaderResourceView>	m_pSRV = nullptr;
		Vector2i							m_size{};
	};
}