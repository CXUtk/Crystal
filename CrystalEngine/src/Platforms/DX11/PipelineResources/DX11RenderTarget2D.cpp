#include "DX11RenderTarget2D.h"
#include "DX11Texture2D.h"

#include "../DX11GraphicsContext.h"
#include "../DX11GraphicsDevice.h"
#include "../dxTrace.h"

namespace crystal
{
	DX11RenderTarget2D::DX11RenderTarget2D(DX11GraphicsDevice* graphicsDevice,
		const RenderTarget2DDescription& desc)
		: m_pGraphicsDevice(graphicsDevice)
	{
		auto device = m_pGraphicsDevice->GetD3DDevice();

		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = static_cast<float>(desc.Size.x);
		m_viewport.Height = static_cast<float>(desc.Size.y);
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;

		bool shaderResource = desc.RTFlags & RenderTargetFlags::CRYSTAL_TEXTURE_TARGET;
		bool depthStencil = (desc.RTFlags & RenderTargetFlags::CRYSTAL_DEPTH_TARGET) || (desc.RTFlags & RenderTargetFlags::CRYSTAL_STENCIL_TARGET);

		if (shaderResource)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
			textureDesc.Width = desc.Size.x;
			textureDesc.Height = desc.Size.y;
			textureDesc.MipLevels = desc.MipmapLevels;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DX11Common::RenderFormatConvert(desc.TargetFormat, true);
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.MiscFlags = 0;
			textureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET
				| D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;


			ComPtr<ID3D11Texture2D> renderTargetTexture = nullptr;
			HR(device->CreateTexture2D(&textureDesc, nullptr, renderTargetTexture.GetAddressOf()));

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			rtvDesc.Format = DX11Common::RenderFormatConvert(desc.TargetFormat, true);
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
			HR(device->CreateRenderTargetView(renderTargetTexture.Get(), &rtvDesc, m_pRenderTargetView.GetAddressOf()));

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DX11Common::RenderFormatConvert(desc.TargetFormat, true);
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipmapLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			HR(device->CreateShaderResourceView(renderTargetTexture.Get(), &srvDesc, m_pShaderResourceView.GetAddressOf()));
		}

		if (depthStencil)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
			textureDesc.Width = desc.Size.x;
			textureDesc.Height = desc.Size.y;
			textureDesc.ArraySize = 1;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.MiscFlags = 0;
			textureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.MipLevels = 1;
			textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

			ComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;
			HR(device->CreateTexture2D(&textureDesc, nullptr, depthStencilTexture.GetAddressOf()));

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			dsvDesc.Flags = 0;
			dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0;

			HR(device->CreateDepthStencilView(depthStencilTexture.Get(), &dsvDesc, m_pDepthStencilView.GetAddressOf()));
		}
	}

	DX11RenderTarget2D::DX11RenderTarget2D(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11RenderTargetView> renderTargetView,
		ComPtr<ID3D11ShaderResourceView> shaderResourceView, ComPtr<ID3D11DepthStencilView> depthStencilView, const D3D11_VIEWPORT& viewPort)
		: m_pGraphicsDevice(graphicsDevice), m_pRenderTargetView(renderTargetView),
		m_pShaderResourceView(shaderResourceView), m_pDepthStencilView(depthStencilView),
		m_viewport(viewPort)
	{
	}


	DX11RenderTarget2D::~DX11RenderTarget2D()
	{}

	void DX11RenderTarget2D::GetShaderResourceHandle(void** pHandle) const
	{
		*pHandle = m_pShaderResourceView.Get();
	}

	Vector2i DX11RenderTarget2D::GetSize() const
	{
		return Vector2i(m_viewport.Width, m_viewport.Height);
	}

	void DX11RenderTarget2D::SetToCurrentContext(DX11GraphicsContext* context)
	{
		context->GetD3DContext()->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	}

	void DX11RenderTarget2D::SetViewportToCurrentContext(DX11GraphicsContext* context)
	{
		context->GetD3DContext()->RSSetViewports(1, &m_viewport);
	}

	void DX11RenderTarget2D::ClearContent(ID3D11DeviceContext* context, ClearOptions options, const Color4f& color, float depth, int stencil)
	{
		if (options & ClearOptions::CRYSTAL_CLEAR_TARGET)
		{
			context->ClearRenderTargetView(m_pRenderTargetView.Get(),
				crystal_value_ptr(color));
		}
		int clearDepthStencilFlags = 0;
		if (options & ClearOptions::CRYSTAL_CLEAR_DEPTH)
		{
			clearDepthStencilFlags |= D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH;
		}
		if (options & ClearOptions::CRYSTAL_CLEAR_STENCIL)
		{
			clearDepthStencilFlags |= D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL;
		}
		if (clearDepthStencilFlags && m_pDepthStencilView != nullptr)
		{
			context->ClearDepthStencilView(m_pDepthStencilView.Get(),
				clearDepthStencilFlags, depth, stencil);
		}
	}
}
