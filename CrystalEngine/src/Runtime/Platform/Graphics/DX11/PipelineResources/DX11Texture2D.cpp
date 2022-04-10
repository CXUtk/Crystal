#include "DX11Texture2D.h"
#include "DX11ShaderProgram.h"
#include "DX11VertexShader.h"
#include "DX11FragmentShader.h"

#include "../DX11GraphicsDevice.h"
#include "../WICTextureLoader.h"
#include "../dxTrace.h"

namespace crystal
{

	DX11Texture2D::DX11Texture2D(DX11GraphicsDevice* graphicsDevice, const std::string& path, const Texture2DDescription& texDesc)
		: m_pGraphicsDevice(graphicsDevice)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.MipLevels = texDesc.MipmapLevels;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DX11Common::RenderFormatConvert(texDesc.Format, true);
		textureDesc.Usage = DX11Common::BufferUsageToDX11Convert(texDesc.Usage);
		textureDesc.MiscFlags = 0;
		textureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		if (texDesc.Usage == BufferUsage::CPURead)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
		}
		else if (texDesc.Usage == BufferUsage::CPUWrite)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
		else if (texDesc.Usage == BufferUsage::CPURWrite)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}

		ComPtr<ID3D11Texture2D> pTexture2D = nullptr;
		DirectX::CreateWICTextureFromFileEx(graphicsDevice->GetD3DDevice(), DX11Common::ConvertFromUtf8ToUtf16(path).c_str(),
			0, textureDesc.Usage, textureDesc.BindFlags, textureDesc.CPUAccessFlags,
			textureDesc.MiscFlags, 0, reinterpret_cast<ID3D11Resource**>(pTexture2D.GetAddressOf()), m_pSRV.GetAddressOf());
		pTexture2D->GetDesc(&textureDesc);

		m_size.x = textureDesc.Width;
		m_size.y = textureDesc.Height;
	}

	DX11Texture2D::DX11Texture2D(DX11GraphicsDevice* graphicsDevice, const uint8_t* src, size_t size, const Texture2DDescription& texDesc)
        : m_pGraphicsDevice(graphicsDevice)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
        textureDesc.Width = texDesc.Size.x;
        textureDesc.Height = texDesc.Size.y;
		textureDesc.MipLevels = texDesc.MipmapLevels;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DX11Common::RenderFormatConvert(texDesc.Format, true);
		textureDesc.Usage = DX11Common::BufferUsageToDX11Convert(texDesc.Usage);
		textureDesc.MiscFlags = 0;
		textureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		if (texDesc.Usage == BufferUsage::CPURead)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
		}
		else if (texDesc.Usage == BufferUsage::CPUWrite)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
		else if (texDesc.Usage == BufferUsage::CPURWrite)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
        ComPtr<ID3D11Texture2D> pTexture2D = nullptr;
        auto device = m_pGraphicsDevice->GetD3DDevice();

        D3D11_SUBRESOURCE_DATA sData = {};
        sData.pSysMem = src;
        sData.SysMemPitch = texDesc.Size.x * DX11Common::RenderFormatToBytes(texDesc.Format);

        HR(device->CreateTexture2D(&textureDesc, &sData, pTexture2D.GetAddressOf()));

        HR(device->CreateShaderResourceView(pTexture2D.Get(), nullptr, m_pSRV.GetAddressOf()));

        m_size.x = textureDesc.Width;
        m_size.y = textureDesc.Height;
	}

	DX11Texture2D::~DX11Texture2D()
	{
    }

	void DX11Texture2D::GetShaderResourceHandle(void* pHandle) const
	{
		ID3D11ShaderResourceView** ptr = (ID3D11ShaderResourceView**)pHandle;
		*ptr = m_pSRV.Get();
	}
}
