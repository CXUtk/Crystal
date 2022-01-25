#pragma once
#include <glm/glm.hpp>

namespace crystal
{
#ifdef CRYSTAL_USE_FLOAT32
	constexpr Float OneMinusEpsilon = 0x1.fffffep-1;
#else
	constexpr Float OneMinusEpsilon = 0x1.fffffffep-1;
#endif



}