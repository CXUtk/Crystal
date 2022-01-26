#pragma once
#include <Platforms/Interfaces/IPlatformProvider.h>
#include <Crystal.h>

namespace crystal
{
	class GLFWGameWindow;

	class GLFWProvider : public IPlatformProvider
	{
	public:
		GLFWProvider(const InitArgs& args);
		~GLFWProvider() override;

		virtual IGameWindow* GetGameWindow() const override;
		virtual IFileSystem* GetFileSystem() const override;

	private:
		std::unique_ptr<GLFWGameWindow> _gameWindow;
		//std::unique_ptr<IFileSystem> _fileSystem;
	};
}