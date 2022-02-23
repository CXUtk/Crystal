#pragma once
#include <Interfaces/Interfaces.h>
#include <Crystal.h>

#include "DX11Common.h"

namespace crystal
{
	class Win32GameWindow;

	class DX11Provider : public IPlatformProvider
	{
	public:
		DX11Provider(const InitArgs& args);
		~DX11Provider() override;

		virtual IGameWindow* GetGameWindow() const override;
		virtual IFileSystem* GetFileSystem() const override;
		virtual IGraphicsDevice* GetGraphicsDevice() const override;
		virtual IGraphicsContext* GetGraphicsContext() const override;
		virtual GraphicsAPIType GetGraphicsAPIType() const override;
		virtual void Present() override;

	private:
		std::unique_ptr<Win32GameWindow>		m_gameWindow;
		std::unique_ptr<DX11GraphicsDevice>		m_dx11GraphicsDevice;
		IGraphicsContext*						m_pDX11GraphicsContext;
	};
}