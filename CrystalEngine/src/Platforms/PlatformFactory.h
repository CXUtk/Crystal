#pragma once
#include <string>
#include <Core/Platform/Platforms.h>

namespace crystal
{
	class PlatformFactory
	{
	public:
		static std::unique_ptr<IPlatformProvider> GetPlatformProvider(const InitArgs& args);
	};
}
