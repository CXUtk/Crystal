#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11Texture2D : public ITexture2D
	{
	public:
		DX11Texture2D(DX11GraphicsDevice* graphicsDevice, const std::string& path, const Texture2DDescription& texDesc);
		DX11Texture2D(DX11GraphicsDevice* graphicsDevice, const void* src, size_t size, const Texture2DDescription& texDesc);
		~DX11Texture2D() override;

		virtual Vector2i GetSize() const override { return m_size; }
		virtual void GetShaderResourceHandle(void* pHandle) const override;

        ID3D11Texture2D* GetTexture2D() const { return m_pTexture2D.Get(); }
	private:
		DX11GraphicsDevice*					m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11ShaderResourceView>	m_pSRV = nullptr;
        ComPtr<ID3D11Texture2D>	            m_pTexture2D = nullptr;
		Vector2i							m_size{};
	};
}