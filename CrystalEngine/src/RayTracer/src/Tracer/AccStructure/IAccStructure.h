#pragma once
#include <memory>
#include <string>
#include <vector>
#include <Core/Math/Geometry.h>
#include <Core/Interaction/SurfaceInteraction.h>

namespace tracer
{
    struct RayHitPayload
    {
        const IRayPrimitive*    HitPrimitive = nullptr;
        Float                   Distance = std::numeric_limits<Float>::infinity();
    };
    class IAccStructure
    {
    public:
        static std::shared_ptr<IAccStructure> CreateAccelerator(const std::string& name);
        virtual ~IAccStructure() = 0 {}

        virtual void Build(const std::vector<std::shared_ptr<IRayPrimitive>>& objects) = 0;
        virtual bool Intersect(const crystal::Ray3f& ray, SurfaceInteraction* info,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;
        virtual bool IntersectTest(const crystal::Ray3f& ray, const IRayPrimitive* ignoreItem,
            float tMin = 0,
            float tMax = std::numeric_limits<float>::infinity()) const = 0;

    protected:
    };
}
