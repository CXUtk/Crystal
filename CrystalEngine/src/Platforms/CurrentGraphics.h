#pragma once
#ifdef CRYSTAL_USE_OPENGL
#include <Platforms/OpenGL/OpenGLPlatform.h>
#elif defined(CRYSTAL_USE_DX11)
#include <Platforms/DX11/DX11Platform.h>
#endif