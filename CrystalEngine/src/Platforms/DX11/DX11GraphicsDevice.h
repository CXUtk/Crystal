#pragma once
#include <Core/Platform/Platforms.h>
#include <Platforms/Windows32/Win32GameWindow.h>

#include "DX11Common.h"

namespace crystal
{
	class DX11GraphicsDevice : public IGraphicsDevice
	{
	public:

		DX11GraphicsDevice(const InitArgs& args, Win32GameWindow* window);
		~DX11GraphicsDevice() override;

		virtual void Clear(ClearOptions options, const Color4f& color, float depth, int stencil) override;
		virtual void Present() override;
		virtual void DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices) override;
		virtual void DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices,
			size_t indexOffset, size_t vertexOffset) override;

		virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const VertexBufferDescription& desc,
			void* src, size_t size) override;
		virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer(const IndexBufferDescription& desc,
			void* src, size_t size) override;
		virtual std::shared_ptr<IVertexShader> CreateVertexShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) override;
		virtual std::shared_ptr<IFragmentShader> CreateFragmentShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) override;
		virtual std::shared_ptr<IShaderProgram> CreateShaderProgramFromFile(const std::string& path) override;

	
		ComPtr<ID3D11Device> GetD3DDevice() const { return m_pd3dDevice; }
		ComPtr<ID3D11DeviceContext> GetD3DDeviceContext() const { return m_pd3dImmediateContext; }
		ComPtr<ID3D11Buffer> CreateBuffer(void* src, size_t size,
			BufferUsage usage, UINT bindFlags);
	private:
		ComPtr<ID3D11Device> m_pd3dDevice;						// D3D11设备
		ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;		// D3D11设备上下文
		ComPtr<IDXGISwapChain> m_pSwapChain;					// D3D11交换链
		ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;		// D3D11 Rendertarget view
		ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;			// D3D11 Depth Stencil buffer
		ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;		// D3D11 Depth Stencil buffer
		D3D11_VIEWPORT m_ScreenViewport;						// 视口

		bool		m_Enable4xMsaa = false;		// 是否开启4倍多重采样
		UINT		m_4xMsaaQuality = 1;		// MSAA支持的质量等级
		Vector2i	m_oldClientSize{};			// History window size, used to detect change


		Win32GameWindow*	m_pWindow;			// Win32 窗体对象

		bool m_initD3DX11();
		void m_resizeBuffer();
		ComPtr<ID3DBlob> m_getShaderBlobFromMemory(const char* src, size_t size, 
			const std::string& name, const std::string& entryPoint, ShaderType type);
	};
}