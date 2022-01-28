#include "PlatformFactory.h"
#ifdef CRYSTAL_USE_GLFW
#include <Platforms/GLFWPlatform.h>
#elif defined(CRYSTAL_USE_DX11)
#include <Platforms/DX11Platform.h>
#endif
namespace crystal
{
	std::unique_ptr<IPlatformProvider> PlatformFactory::GetPlatformProvider(const InitArgs& args)
	{
#ifdef CRYSTAL_USE_GLFW
		return std::make_unique<GLFWProvider>(args);
#elif defined(CRYSTAL_USE_DX11)
		return std::make_unique<DX11Provider>(args);
#endif
	}
}
