#pragma once
#include <Platforms/Interfaces/IPlatformProvider.h>
#include <Crystal.h>

namespace crystal
{
	class GLFWProvider : public IPlatformProvider
	{
	public:
		GLFWProvider(const InitArgs& args);
		~GLFWProvider() override;

		virtual IGameWindow* GetGameWindow() const override { return ptr(_gameWindow); }
		virtual IFileSystem* GetFileSystem() const override { return ptr(_fileSystem); }

	private:
		std::unique_ptr<IGameWindow> _gameWindow;
		std::unique_ptr<IFileSystem> _fileSystem;
	};
}