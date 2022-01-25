#pragma once
#include <Platforms/Platforms.h>

namespace crystal
{
	class IPlatformProvider
	{
	public:
		virtual ~IPlatformProvider() = 0 {};
		virtual IGameWindow* GetGameWindow() const = 0;
		virtual IFileSystem* GetFileSystem() const = 0;
	};
}