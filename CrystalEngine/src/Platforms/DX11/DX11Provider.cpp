#include "DX11Provider.h"
#include <Platforms/Windows32/Win32GameWindow.h>
#include <Platforms/DX11/DX11GraphicsDevice.h>

namespace crystal
{
	DX11Provider::DX11Provider(const InitArgs& args)
	{
		_gameWindow = std::make_unique<Win32GameWindow>(args, GetModuleHandle(NULL));
		_dx11GraphicsDevice = std::make_unique<DX11GraphicsDevice>(args, ptr(_gameWindow));
	}

	DX11Provider::~DX11Provider()
	{}

	IGameWindow* DX11Provider::GetGameWindow() const
	{
		return ptr(_gameWindow);
	}

	IFileSystem* DX11Provider::GetFileSystem() const
	{
		return nullptr;
	}
}
