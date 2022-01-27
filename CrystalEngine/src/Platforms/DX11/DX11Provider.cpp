#include "DX11Provider.h"
#include <Platforms/Windows32/Win32GameWindow.h>

namespace crystal
{
	DX11Provider::DX11Provider(const InitArgs& args)
	{
		_gameWindow = std::make_unique< Win32GameWindow>(args, GetModuleHandle(NULL));
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
