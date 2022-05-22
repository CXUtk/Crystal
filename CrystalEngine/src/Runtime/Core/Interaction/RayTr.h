#pragma once
#include <Core/Math/Geometry.h>


namespace crystal
{
    class Medium;
    struct RayTr
    {
        Ray3f           Ray{};
        const Medium*   Medium{};
    };
}