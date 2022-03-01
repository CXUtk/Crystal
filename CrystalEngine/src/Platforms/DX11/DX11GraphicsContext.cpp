#include "DX11GraphicsContext.h"
#include "DX11GraphicsDevice.h"
#include "DX11PipelineResourceObject.h"
#include "DX11PipelineStateObject.h"

#include "PipelineResources/DX11RenderTarget2D.h"

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
		m_renderTargets.push_back(nullptr);
		m_CreateSwapChainAndLink(args);
		m_ResizeBuffer();
		m_pWindow->AppendOnResizeEvent([this](Vector2i size) {
			m_ResizeBuffer();
		});

	}

	DX11GraphicsContext::~DX11GraphicsContext()
	{}

	void DX11GraphicsContext::ReturnLastViewports()
	{
		m_renderTargets.back()->SetViewportToCurrentContext(this);
	}

	void DX11GraphicsContext::Present()
	{
		HR(m_pSwapChain->Present(0, 0));
	}

	void DX11GraphicsContext::DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices)
	{
		if (!m_pCurrentPipelineResource || !m_pCurrentPipelineState)
		{
			throw std::logic_error("Cannot start a draw when PipelineResource or PipelineState are not active");
		}
		m_pd3dImmediateContext->IASetPrimitiveTopology(DX11Common::PrimitiveTypeToTopologyConvert(primitiveType));
		m_pd3dImmediateContext->Draw(numVertices, offset);
	}

	void DX11GraphicsContext::DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices, size_t indexOffset, size_t vertexOffset)
	{
		if (!m_pCurrentPipelineResource || !m_pCurrentPipelineState)
		{
			throw std::logic_error("Cannot start a draw when PipelineResource or PipelineState are not active");
		}
		m_pd3dImmediateContext->IASetPrimitiveTopology(DX11Common::PrimitiveTypeToTopologyConvert(primitiveType));
		m_pd3dImmediateContext->DrawIndexed(numIndices, indexOffset, vertexOffset);
	}

	void DX11GraphicsContext::m_ResizeBuffer()
	{
		auto dx11GraphicsDevice = m_pGraphicsDevice->GetD3DDevice();
		assert(m_pd3dImmediateContext);
		assert(dx11GraphicsDevice);
		assert(m_pSwapChain);

        m_renderTargets[0].reset();

		// Resize the buffer if window size was changed
		auto newWindowSize = m_pWindow->GetWindowSize();
		if (m_backBufferSize != newWindowSize)
		{

			HR(m_pSwapChain->ResizeBuffers(1, newWindowSize.x, newWindowSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
			m_backBufferSize = newWindowSize;
		}

		// Create the render target view
		ComPtr<ID3D11RenderTargetView> renderTargetView;
		{
			ComPtr<ID3D11Texture2D> backBuffer;

			m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
			dx11GraphicsDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());

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
		ComPtr<ID3D11Texture2D>	depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		HR(dx11GraphicsDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf()));
		HR(dx11GraphicsDevice->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.ReleaseAndGetAddressOf()));

		// Combine render targets and the depth stencil buffer to the pipeline
		m_pd3dImmediateContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

		D3D11_VIEWPORT screenViewport{};
		// Initialize viewport
		screenViewport.TopLeftX = 0;
		screenViewport.TopLeftY = 0;
		screenViewport.Width = static_cast<float>(newWindowSize.x);
		screenViewport.Height = static_cast<float>(newWindowSize.y);
		screenViewport.MinDepth = 0.0f;
		screenViewport.MaxDepth = 1.0f;

		m_pd3dImmediateContext->RSSetViewports(1, &screenViewport);

		// Set DEBUG name
		d3dUtils::D3D11SetDebugObjectName(depthStencilBuffer.Get(), "DepthStencilBuffer");
		d3dUtils::D3D11SetDebugObjectName(depthStencilView.Get(), "DepthStencilView");
		d3dUtils::D3D11SetDebugObjectName(renderTargetView.Get(), "BackBufferRTV[0]");

        m_renderTargets[0].reset();
		m_renderTargets[0] = std::make_shared<DX11RenderTarget2D>(m_pGraphicsDevice, renderTargetView,
			nullptr, depthStencilView, screenViewport);
	}

	void DX11GraphicsContext::m_CreateSwapChainAndLink(const InitArgs& args)
	{
		auto dx11GraphicsDevice = m_pGraphicsDevice->GetD3DDevice();

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
	}

	void DX11GraphicsContext::Clear(ClearOptions options, const Color4f& color, float depth, int stencil)
	{
		m_renderTargets.back()->ClearContent(m_pd3dImmediateContext.Get(),
			options, color, depth, stencil);
	}

	void DX11GraphicsContext::BeginPipeline(std::shared_ptr<IPipelineStateObject> pipelineState)
	{
		if (m_pCurrentPipelineState)
		{
			throw std::logic_error("Cannot begin a pipeline when a pipeline already started");
		}
		m_pCurrentPipelineState = std::dynamic_pointer_cast<DX11PipelineStateObject>(pipelineState);
		m_pCurrentPipelineState->Load();
	}

	void DX11GraphicsContext::EndPipeline()
	{
		if (!m_pCurrentPipelineState)
		{
			throw std::logic_error("Cannot end a pipeline when there is no pipeline process");
		}
		m_pCurrentPipelineState->Unload();
		m_pCurrentPipelineState.reset();
	}

	void DX11GraphicsContext::LoadPipelineResources(std::shared_ptr<IPipelineResourceObject> pipelineResource)
	{
		if (m_pCurrentPipelineResource)
		{
			throw std::logic_error("Cannot load pipeline resource when there is a resource in active");
		}
		m_pCurrentPipelineResource = std::dynamic_pointer_cast<DX11PipelineResourceObject>(pipelineResource);
		m_pCurrentPipelineResource->Load();
	}

	void DX11GraphicsContext::UnloadPipelineResources()
	{
		if (!m_pCurrentPipelineResource)
		{
			throw std::logic_error("Cannot unload resource when there is no resource loaded");
		}
		m_pCurrentPipelineResource->Unload();
		m_pCurrentPipelineResource.reset();
	}


	void DX11GraphicsContext::PushRenderTarget2D(std::shared_ptr<IRenderTarget2D> renderTarget2D)
	{
		assert(m_renderTargets.size() > 0);
		auto dx11RenderTarget = std::dynamic_pointer_cast<DX11RenderTarget2D>(renderTarget2D);
		m_renderTargets.push_back(dx11RenderTarget);

		m_renderTargets.back()->SetViewportToCurrentContext(this);
		m_renderTargets.back()->SetToCurrentContext(this);
	}

	void DX11GraphicsContext::PopRenderTarget2D()
	{
		assert(m_renderTargets.size() > 1);
		m_renderTargets.pop_back();
		m_renderTargets.back()->SetViewportToCurrentContext(this);
		m_renderTargets.back()->SetToCurrentContext(this);
	}
	Vector2i DX11GraphicsContext::GetCurrentFrameBufferSize() const
	{
		return m_renderTargets.back()->GetSize();
	}
}
