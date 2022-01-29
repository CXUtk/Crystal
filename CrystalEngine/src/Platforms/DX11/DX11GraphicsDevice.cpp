#include "DX11GraphicsDevice.h"
#include "d3dUtils.h"
#include "dxTrace.h"

#include <Core/InitArgs.h>

namespace crystal
{
	DX11GraphicsDevice::DX11GraphicsDevice(const InitArgs& args, Win32GameWindow* window)
		: m_Window(window), m_Enable4xMsaa(args.Enable4xMSAA)
	{
		if (!initD3DX11())
		{
			throw std::exception("[DX11GraphicsDevice::DX11GraphicsDevice] Unable to start Dx11");
		}
	}

	DX11GraphicsDevice::~DX11GraphicsDevice()
	{}

	bool DX11GraphicsDevice::initD3DX11()
	{
		HRESULT hr = S_OK;

		// Create D3D device and D3D device context
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// ������������
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		// ���Եȼ�����
		D3D_FEATURE_LEVEL featureLevels[1] = { D3D_FEATURE_LEVEL_11_0 };
		UINT numFeatureLevels = 1;

		D3D_FEATURE_LEVEL curFeatureLevel;
		D3D_DRIVER_TYPE d3dDriverType;

		d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(),
			&curFeatureLevel, m_pd3dImmediateContext.GetAddressOf());

		if (FAILED(hr))
		{
			MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
			return false;
		}

		// ����Ƿ�֧�����Եȼ�11.0
		if (curFeatureLevel != D3D_FEATURE_LEVEL_11_0 && curFeatureLevel != D3D_FEATURE_LEVEL_11_1)
		{
			MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		// ��� MSAA֧�ֵ������ȼ�
		m_pd3dDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
		assert(m_4xMsaaQuality > 0);


		ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
		ComPtr<IDXGIFactory> dxgiFactory = 0;
		HR(m_pd3dDevice.As(&dxgiDevice));
		HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
		HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())));

		auto clientSize = m_Window->GetWindowSize();
		m_oldClientSize = clientSize;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = clientSize.x;
		sd.BufferDesc.Height = clientSize.y;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// �Ƿ���4�����ز�����
		if (m_Enable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_Window->GetHWND();
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		HR(dxgiFactory->CreateSwapChain(m_pd3dDevice.Get(), &sd, m_pSwapChain.GetAddressOf()));

		// ���õ��Զ�����
		D3D11SetDebugObjectName(m_pd3dImmediateContext.Get(), "ImmediateContext");
		DXGISetDebugObjectName(m_pSwapChain.Get(), "SwapChain");

		m_resizeBuffer();
		return true;
	}

	void DX11GraphicsDevice::m_resizeBuffer()
	{
		assert(m_pd3dImmediateContext);
		assert(m_pd3dDevice);
		assert(m_pSwapChain);

		// Release previous buffer resources
		m_pRenderTargetView.Reset();
		m_pDepthStencilView.Reset();
		m_pDepthStencilBuffer.Reset();

		// Resize the buffer if window size was changed
		auto newWindowSize = m_Window->GetWindowSize();
		if (m_oldClientSize != newWindowSize)
		{
			HR(m_pSwapChain->ResizeBuffers(1, newWindowSize.x, newWindowSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
			m_oldClientSize = newWindowSize;
		}

		// Create the render target view
		{
			ComPtr<ID3D11Texture2D> backBuffer;

			m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
			m_pd3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());

			// Set DEBUG name
			D3D11SetDebugObjectName(backBuffer.Get(), "BackBuffer[0]");
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
			depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
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
		HR(m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf()));
		HR(m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf()));
	
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
		D3D11SetDebugObjectName(m_pDepthStencilBuffer.Get(), "DepthStencilBuffer");
		D3D11SetDebugObjectName(m_pDepthStencilView.Get(), "DepthStencilView");
		D3D11SetDebugObjectName(m_pRenderTargetView.Get(), "BackBufferRTV[0]");
	}
}