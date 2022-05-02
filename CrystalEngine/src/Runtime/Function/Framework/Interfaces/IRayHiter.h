#pragma once
#include <Function/Framework/Object/GameObject.h>
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    class IRayHiter
    {
    public:
        virtual ~IRayHiter() = 0 {};

        virtual const GameObject* GetObject() const = 0;

        // Get bounding box of transformed model
        virtual Bound3f GetBoundingBox() const = 0;

        // Do ray-model intersection test, return surface interaction info
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const = 0;

        // Do ray-model intersection test, but only test if there is an intersection
        virtual bool IntersectTest(const Ray3f& ray, float tMin = 0,
            float tMax = std::numeric_limits<float>::infinity()) const = 0;
    };
}