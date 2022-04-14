#include "ImageSampling.h"

namespace crystal
{
    CubeUV crystal::XYZToCubeUV(const Vector3f& p)
    {
        float ax = std::abs(p.x);
        float ay = std::abs(p.y);
        float az = std::abs(p.z);
        CubeUV c;
        if (ax >= ay && ax >= az)
        {
            // x face
            c = { p.x >= 0 ? 0 : 1, Vector2f(-p.z / p.x, p.y / ax) };
        }
        else if (ay >= az)
        {
            // y face
            c = { p.y >= 0 ? 2 : 3, Vector2f(p.x / ay, -p.z / p.y) };
        }
        else
        {
            // z face
            c = { p.z >= 0 ? 4 : 5, Vector2f(p.x / p.z, p.y / az) };
        }
        assert(glm::isnan(c.UV) == glm::bvec2(false));
        c.UV.x = c.UV.x * 0.5f + 0.5f;
        c.UV.y = c.UV.y * 0.5f + 0.5f;
        return c;
    }

    Vector3f CubeUVToVector(const CubeUV& cube)
    {
        auto pos = cube.UV * 2.f - Vector2f(1.f);

        Vector3f vector;
        if (cube.Id < 2)
        {
            float neg = (cube.Id == 0) ? 1.f : -1.f;
            vector = Vector3f(neg, pos.y, -neg * pos.x);
        }
        else if (cube.Id < 4)
        {
            float neg = (cube.Id == 2) ? 1.f : -1.f;
            vector = Vector3f(pos.x, neg, -neg * pos.y);
        }
        else
        {
            float neg = (cube.Id == 4) ? 1.f : -1.f;
            vector = Vector3f(neg * pos.x, pos.y, neg);
        }

        vector = glm::normalize(vector);
        assert(glm::isnan(vector) == glm::bvec3(false));
        return vector;
    }


    Vector3f sRGBToHDR(const Vector3f& color)
    {
        return glm::pow(color, Vector3f(2.2f));
    }

    Vector3f HDRTosRGB(const Vector3f& hdr)
    {
        return glm::pow(hdr, Vector3f(1 / 2.2f));
    }
}
