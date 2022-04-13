#pragma once
#include "../DX11Common.h"
#include "DX11Texture2D.h"
#include <Platform/RHI/FileSystem/FSCommon.h>

namespace crystal
{
	class DX11TextureCubemap : public ITextureCubemap
	{
	public:
        DX11TextureCubemap(DX11GraphicsDevice* graphicsDevice,
            const CubemapTexture6& texture6, const Texture2DDescription& texDesc);
        DX11TextureCubemap(DX11GraphicsDevice* graphicsDevice,
            const uint8_t* src, size_t size, const Texture2DDescription& texDesc);
		~DX11TextureCubemap() override;

		virtual void GetShaderResourceHandle(void* pHandle) const override;
	private:
		DX11GraphicsDevice*					m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11ShaderResourceView>	m_pSRV = nullptr;
	};
}