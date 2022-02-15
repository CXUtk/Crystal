#pragma once
#include "DX11Common.h"

namespace crystal
{
	class Win32GameWindow;

	class DX11GraphicsContext : public IGraphicsContext
	{
	public:
		DX11GraphicsContext(DX11GraphicsDevice* graphicsDevice, Win32GameWindow* window,
			ComPtr<ID3D11DeviceContext> context, const InitArgs& args);
		~DX11GraphicsContext() override;

		ID3D11DeviceContext* GetD3DDevice() const { return m_pd3dImmediateContext.Get(); }

		virtual void Present() override;
		virtual void DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices) override;
		virtual void DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices,
			size_t indexOffset, size_t vertexOffset) override;
		virtual void Clear(ClearOptions options, const Color4f& color, float depth, int stencil) override;

		virtual void PushPipeline(std::shared_ptr<IPipelineResourceObject> pipelineResource,
			std::shared_ptr<IPipelineStateObject> pipelineState) override;
		virtual void PopPipeline() override;
		virtual void PushRenderTarget2D(std::shared_ptr<IRenderTarget2D> renderTarget2D) override;
		virtual void PopRenderTarget2D() override;

	private:

		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11DeviceContext>		m_pd3dImmediateContext = nullptr;

		ComPtr<IDXGISwapChain>			m_pSwapChain = nullptr;
		//ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView = nullptr;
		//ComPtr<ID3D11Texture2D>			m_pDepthStencilBuffer = nullptr;
		//ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;

		D3D11_VIEWPORT					m_ScreenViewport{};
		UINT							m_MSAAQuality = 0;
		bool							m_Enable4xMsaa = false;
		Vector2i						m_backBufferSize{};

		std::vector<std::shared_ptr<DX11RenderTarget2D>>	m_renderTargets;

		Win32GameWindow*				m_pWindow = nullptr;

		void m_resizeBuffer();
		void m_CreateSwapChainAndLink(const InitArgs& args);
	};
}