#include "DX11GraphicsContext.h"
#include "DX11GraphicsDevice.h"
#include "DX11RenderTarget2D.h"

#include <Platforms/DX11/dxTrace.h>
#include <Platforms/Windows32/Win32GameWindow.h>
#include <Core/InitArgs.h>
#include <Platforms/DX11/d3dUtils.h>

namespace crystal
{
	DX11GraphicsContext::DX11GraphicsContext(DX11GraphicsDevice* graphicsDevice, Win32GameWindow* window,
		ComPtr<ID3D11DeviceContext> context, const InitArgs& args)
		: m_pGraphicsDevice(graphicsDevice), m_pWindow(window), m_pd3dImmediateContext(context)
	{
		auto dx11GraphicsDevice = graphicsDevice->GetD3DDevice();

		// 检测 MSAA支持的质量等级
		dx11GraphicsDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MSAAQuality);
		assert(m_MSAAQuality > 0);

		m_Enable4xMsaa = args.Enable4xMSAA;

		ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
		ComPtr<IDXGIFactory> dxgiFactory = 0;
		HR(dx11GraphicsDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
		HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
		HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())));

		auto clientSize = m_pWindow->GetWindowSize();

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = clientSize.x;
		sd.BufferDesc.Height = clientSize.y;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// 是否开启4倍多重采样？
		if (args.Enable4xMSAA)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_MSAAQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_pWindow->GetHWND();
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		HR(dxgiFactory->CreateSwapChain(dx11GraphicsDevice, &sd, m_pSwapChain.GetAddressOf()));

		dxgiFactory->MakeWindowAssociation(m_pWindow->GetHWND(), DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

		// 设置调试对象名
		d3dUtils::D3D11SetDebugObjectName(m_pd3dImmediateContext.Get(), "ImmediateContext");
		d3dUtils::DXGISetDebugObjectName(m_pSwapChain.Get(), "SwapChain");


		m_resizeBuffer();
		m_pWindow->AppendOnResizeEvent([this](Vector2i size) {
			m_resizeBuffer();
		});

	}

	DX11GraphicsContext::~DX11GraphicsContext()
	{
	}

	void DX11GraphicsContext::Present()
	{
		HR(m_pSwapChain->Present(0, 0));
	}

	void DX11GraphicsContext::m_resizeBuffer()
	{
		auto dx11GraphicsDevice = m_pGraphicsDevice->GetD3DDevice();
		assert(m_pd3dImmediateContext);
		assert(dx11GraphicsDevice);
		assert(m_pSwapChain);

		// Release previous buffer resources
		m_pRenderTargetView.Reset();
		m_pDepthStencilView.Reset();
		m_pDepthStencilBuffer.Reset();

		// Resize the buffer if window size was changed
		auto newWindowSize = m_pWindow->GetWindowSize();
		if (m_backBufferSize != newWindowSize)
		{
			HR(m_pSwapChain->ResizeBuffers(1, newWindowSize.x, newWindowSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
			m_backBufferSize = newWindowSize;
		}

		// Create the render target view
		{
			ComPtr<ID3D11Texture2D> backBuffer;

			m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.ReleaseAndGetAddressOf()));
			dx11GraphicsDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.ReleaseAndGetAddressOf());

			// Set DEBUG name
			d3dUtils::D3D11SetDebugObjectName(backBuffer.Get(), "BackBuffer[0]");
		}

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = newWindowSize.x;
		depthStencilDesc.Height = newWindowSize.y;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// Check MSAA state
		if (m_Enable4xMsaa)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = m_MSAAQuality - 1;
		}
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}

		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		// Create DepthStencil buffer and view
		HR(dx11GraphicsDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.ReleaseAndGetAddressOf()));
		HR(dx11GraphicsDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.ReleaseAndGetAddressOf()));

		// Combine render targets and the depth stencil buffer to the pipeline
		m_pd3dImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

		// Initialize viewport
		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = static_cast<float>(newWindowSize.x);
		m_ScreenViewport.Height = static_cast<float>(newWindowSize.y);
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;

		m_pd3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

		// Set DEBUG name
		d3dUtils::D3D11SetDebugObjectName(m_pDepthStencilBuffer.Get(), "DepthStencilBuffer");
		d3dUtils::D3D11SetDebugObjectName(m_pDepthStencilView.Get(), "DepthStencilView");
		d3dUtils::D3D11SetDebugObjectName(m_pRenderTargetView.Get(), "BackBufferRTV[0]");
	}

	void DX11GraphicsContext::Clear(ClearOptions options, const Color4f& color, float depth, int stencil)
	{
		if (options & ClearOptions::CRYSTAL_CLEAR_TARGET)
		{
			// On render target null
			if (m_renderTargetStackPtr == 0)
			{
				m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(),
					crystal_value_ptr(color));
			}
			else
			{
				m_pd3dImmediateContext->ClearRenderTargetView(m_renderTargets[m_renderTargetStackPtr]->GetRenderTargetView(),
					crystal_value_ptr(color));
			}
		}

		int clearFlag = 0;
		if (options & ClearOptions::CRYSTAL_CLEAR_DEPTH)
		{
			clearFlag |= D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH;
		}
		if (options & ClearOptions::CRYSTAL_CLEAR_STENCIL)
		{
			clearFlag |= D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL;
		}
		if (clearFlag)
		{
			if (m_renderTargetStackPtr == 0)
			{
				m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), clearFlag, depth, stencil);
			}
			else
			{
				auto depthStencilView = m_renderTargets[m_renderTargetStackPtr]->GetDepthStencilView();
				if (depthStencilView)
				{
					m_pd3dImmediateContext->ClearDepthStencilView(depthStencilView,
						clearFlag, depth, stencil);
				}
			}

		}
	}
}
