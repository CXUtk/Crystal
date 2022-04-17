#pragma once
#include <Crystal.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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

    using Matrix2f = glm::mat2;
    using Matrix3f = glm::mat3;
    using Matrix4f = glm::mat4;

    using Quaternion = glm::quat;
}