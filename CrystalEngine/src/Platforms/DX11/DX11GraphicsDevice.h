#pragma once
#include <Core/Platform/Platforms.h>
#include <Platforms/Windows32/Win32GameWindow.h>

#include <d3d11_1.h>

namespace crystal
{
	class DX11GraphicsDevice : public IGraphicsDevice
	{
	public:
		DX11GraphicsDevice(const InitArgs& args, Win32GameWindow* window);
		~DX11GraphicsDevice() override;

	private:
		// 使用模板别名(C++11)简化类型名
		template <class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

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


		Win32GameWindow* m_Window;

		bool initD3DX11();
		void m_resizeBuffer();
	};
}