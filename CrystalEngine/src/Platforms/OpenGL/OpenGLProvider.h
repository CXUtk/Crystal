#pragma once
#include "OpenGLCommon.h"
namespace crystal
{
	class GLFWGameWindow;
	class OpenGLProvider : public IPlatformProvider
	{
	public:
		OpenGLProvider(const InitArgs& args);
		~OpenGLProvider() override;

		virtual IGameWindow* GetGameWindow() const override;
		virtual IFileSystem* GetFileSystem() const override;
		virtual IGraphicsDevice* GetGraphicsDevice() const override;

	private:
		std::unique_ptr<GLFWGameWindow> m_gameWindow;
		//std::unique_ptr<IFileSystem> _fileSystem;
		std::unique_ptr<OpenGLGraphicsDevice> m_graphicsDevice;

		void mountDebugErrorLog();
	};
}