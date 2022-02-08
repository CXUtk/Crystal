#include "DX11Provider.h"
#include <Platforms/Windows32/Win32GameWindow.h>
#include <Platforms/DX11/DX11GraphicsDevice.h>

namespace crystal
{
	DX11Provider::DX11Provider(const InitArgs& args)
	{
		m_gameWindow = std::make_unique<Win32GameWindow>(args, GetModuleHandle(NULL));
		m_dx11GraphicsDevice = std::make_unique<DX11GraphicsDevice>(args, ptr(m_gameWindow));
		DX11Common::InitDX11Commons(ptr(m_dx11GraphicsDevice));
	}

	DX11Provider::~DX11Provider()
	{
		ComPtr<ID3D11Debug> d3dDebug;
		m_dx11GraphicsDevice->GetD3DDevice()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(d3dDebug.GetAddressOf()));

		d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}

	IGameWindow* DX11Provider::GetGameWindow() const
	{
		return ptr(m_gameWindow);
	}

	IFileSystem* DX11Provider::GetFileSystem() const
	{
		return nullptr;
	}
	IGraphicsDevice* DX11Provider::GetGraphicsDevice() const
	{
		return ptr(m_dx11GraphicsDevice);
	}
}
