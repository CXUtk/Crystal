#pragma once
#include <Core/Math/Geometry.h>
#include <Platform/RHI/Graphics/GraphicsCommon.h>

namespace crystal
{
    struct CubeUV
    {
        int         Id;
        Vector2f    UV;
    };

    CubeUV XYZToCubeUV(const Vector3f& p);
    Vector3f CubeUVToVector(const CubeUV& cube);

    inline Vector3f sRGBToHDR(const Vector3f& color)
    {
        return glm::pow(color, Vector3f(2.2f));
    }

    inline Vector3f HDRTosRGB(const Vector3f& hdr)
    {
        return glm::pow(hdr, Vector3f(1 / 2.2f));
    }
}