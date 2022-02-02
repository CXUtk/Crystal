#pragma once
#include <Crystal.h>
#include <glm/glm.hpp>

namespace crystal
{
#ifdef CRYSTAL_USE_FLOAT64
    constexpr Float OneMinusEpsilon = 0x1.fffffffffffffp-1;
#else
    constexpr Float OneMinusEpsilon = 0x1.fffffep-1;
#endif

    using Point2f = glm::vec2;
    using Point2i = glm::ivec2;
    using Point3f = glm::vec3;
    using Vector2i = glm::ivec2;
    using Vector2f = glm::vec2;
    using Vector3f = glm::vec3;
    using Vector4f = glm::vec4;
    using Normal3f = glm::vec3;
}