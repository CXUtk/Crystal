#include "DX11Provider.h"

#include "Platform/System/Windows32/Win32GameWindow.h"
#include "DX11GraphicsDevice.h"
#include "DX11GraphicsContext.h"

#include <Core/Utils/Logger.h>
#include <Core/Utils/Misc.h>

namespace crystal
{
	DX11Provider::DX11Provider()
	{
		m_gameWindow = std::make_unique<Win32GameWindow>(GetModuleHandle(NULL));
		m_dx11GraphicsDevice = std::make_unique<DX11GraphicsDevice>(ptr(m_gameWindow));
		m_pDX11GraphicsContext = ptr(m_dx11GraphicsDevice->GetContext());
		DX11Common::InitDX11Commons(ptr(m_dx11GraphicsDevice));
	}

	DX11Provider::~DX11Provider()
	{
#ifdef _DEBUG
		ComPtr<ID3D11Debug> d3dDebug;
		m_dx11GraphicsDevice->GetD3DDevice()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(d3dDebug.GetAddressOf()));

		d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif // DEBUG
	}

	IGameWindow* DX11Provider::GetGameWindow() const
	{
		return ptr(m_gameWindow);
	}

	IFileSystem* DX11Provider::GetFileSystem() const
	{
		return nullptr;
	}

	void DX11Provider::LoadSharedLibrary(const std::string& name, void* handle)
	{
        HMODULE hDll = LoadLibraryW(DX11Common::ConvertFromUtf8ToUtf16(name).c_str());
        if (!hDll || hDll == INVALID_HANDLE_VALUE)
        {
            throw std::logic_error(string_format("Unable to load library %s", name.c_str()));
        }
        *((HMODULE*)handle) = hDll;
	}

    bool DX11Provider::ReleaseSharedLibrary(void* handle)
    {
        HMODULE hDll = *((HMODULE*)handle);
        return FreeLibrary(hDll);
    }

	IGraphicsDevice* DX11Provider::GetGraphicsDevice() const
	{
		return ptr(m_dx11GraphicsDevice);
	}

	IGraphicsContext* DX11Provider::GetGraphicsContext() const
	{
		return m_pDX11GraphicsContext;
	}

	GraphicsAPIType DX11Provider::GetGraphicsAPIType() const
	{
		return GraphicsAPIType::DirectX11;
	}

	void DX11Provider::Present()
	{
		m_pDX11GraphicsContext->Present();
	}
}
