#pragma once
#include <string>
#include "Platform/RHI/Interfaces.h"
#include "Resource/Config/AppInitArguments.h"

namespace crystal
{
	class PlatformFactory
	{
	public:
		static std::unique_ptr<IPlatformProvider> GetPlatformProvider();
	};
}
