#pragma once
#include <memory>
#include <string>
#include <vector>
#include <Core/Math/Geometry.h>
#include <Core/Interaction/SurfaceInteraction.h>
#include <Function/Framework/Object/IRayHiter.h>

namespace tracer
{
    class IAccStructure
    {
    public:
        static std::shared_ptr<IAccStructure> CreateAccelerator(const std::string& name);
        virtual ~IAccStructure() = 0 {}

        virtual void Build(const std::vector<std::shared_ptr<IRayHiter>>& objects) = 0;
        virtual bool Intersect(const crystal::Ray3f& ray, SurfaceInteraction* info,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;
        virtual bool IntersectTest(const crystal::Ray3f& ray, const IRayHiter* ignoreShape,
            float tMin = 0,
            float tMax = std::numeric_limits<float>::infinity()) const = 0;

    protected:
    };
}
