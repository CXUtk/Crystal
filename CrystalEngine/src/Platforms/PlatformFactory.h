#pragma once
#include <string>
#include <Core/Platform/IPlatformProvider.h>
namespace crystal
{
	class PlatformFactory
	{
	public:
		static std::unique_ptr<IPlatformProvider> GetPlatformProvider(const InitArgs& args);
	};
}
