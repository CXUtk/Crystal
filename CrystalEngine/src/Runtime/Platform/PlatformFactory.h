#pragma once
#include <string>
#include "Platform/RHI/Interfaces.h"

namespace crystal
{
	class PlatformFactory
	{
	public:
		static std::unique_ptr<IPlatformProvider> GetPlatformProvider(const InitArgs& args);
	};
}
