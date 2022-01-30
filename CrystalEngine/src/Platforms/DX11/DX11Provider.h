#pragma once
#include <Core/Platform/Platforms.h>
#include <Crystal.h>

#include "DX11Common.h"

namespace crystal
{
	class Win32GameWindow;
	class DX11GraphicsDevice;

	class DX11Provider : public IPlatformProvider
	{
	public:
		DX11Provider(const InitArgs& args);
		~DX11Provider() override;

		virtual IGameWindow* GetGameWindow() const override;
		virtual IFileSystem* GetFileSystem() const override;
		virtual IGraphicsDevice* GetGraphicsDevice() const override;

	private:
		std::unique_ptr<Win32GameWindow> _gameWindow;
		std::unique_ptr<DX11GraphicsDevice> m_dx11GraphicsDevice;
	};
}