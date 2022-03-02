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

		ID3D11DeviceContext* GetD3DContext() const { return m_pd3dImmediateContext.Get(); }
		Vector2i GetBackBufferSize() const { return m_backBufferSize; }
		void ReturnLastViewports();

		virtual void Present() override;
		virtual void DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices) override;
		virtual void DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices,
			size_t indexOffset, size_t vertexOffset) override;
		virtual void Clear(ClearOptions options, const Color4f& color, float depth, int stencil) override;

		virtual void BeginPipeline(std::shared_ptr<IPipelineStateObject> pipelineState) override;
		virtual void EndPipeline() override;
		
		virtual void LoadPipelineResources(std::shared_ptr<IPipelineResourceObject> pipelineResource) override;
		virtual void UnloadPipelineResources() override;

		virtual void PushRenderTarget2D(std::shared_ptr<IRenderTarget2D> renderTarget2D) override;
		virtual void PopRenderTarget2D() override;

		virtual Vector2i GetCurrentFrameBufferSize() const override;
	private:

        ComPtr<ID3D11Device>		    m_pd3dGraphicsDevice = nullptr;
		ComPtr<ID3D11DeviceContext>		m_pd3dImmediateContext = nullptr;
		ComPtr<IDXGISwapChain>			m_pSwapChain = nullptr;

		//ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView = nullptr;
		//ComPtr<ID3D11Texture2D>			m_pDepthStencilBuffer = nullptr;
		//ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;

		UINT							m_MSAAQuality = 0;
		bool							m_Enable4xMsaa = false;
		Vector2i						m_backBufferSize{};
		Win32GameWindow*				m_pWindow = nullptr;

		std::vector<std::shared_ptr<DX11RenderTarget2D>>	m_renderTargets;
		std::shared_ptr<DX11PipelineResourceObject>			m_pCurrentPipelineResource;
		std::shared_ptr<DX11PipelineStateObject>			m_pCurrentPipelineState;

		void m_ResizeBuffer(DX11GraphicsDevice* graphicsDevice);
		void m_CreateSwapChainAndLink(const InitArgs& args);
	};
}