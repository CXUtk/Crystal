#include "PlatformFactory.h"
#include "CurrentGraphics.h"

namespace crystal
{
	std::unique_ptr<IPlatformProvider> PlatformFactory::GetPlatformProvider(const InitArgs& args)
	{
#ifdef CRYSTAL_USE_OPENGL
		return std::make_unique<OpenGLProvider>(args);
#elif defined(CRYSTAL_USE_DX11)
		return std::make_unique<DX11Provider>(args);
#endif
	}
}
