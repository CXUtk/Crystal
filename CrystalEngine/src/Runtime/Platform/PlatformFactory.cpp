#include "PlatformFactory.h"
#ifdef CRYSTAL_USE_OPENGL
#include "Graphics/OpenGL/OpenGLPlatform.h"
#endif
#ifdef CRYSTAL_USE_DX11
#include "Graphics/DX11/DX11Platform.h"
#endif

namespace crystal
{
	std::unique_ptr<IPlatformProvider> PlatformFactory::GetPlatformProvider(const InitArgs& args)
	{
#ifdef CRYSTAL_USE_OPENGL
		return std::make_unique<OpenGLProvider>(args);
#elif CRYSTAL_USE_DX11
		return std::make_unique<DX11Provider>(args);
#endif
	}
}
