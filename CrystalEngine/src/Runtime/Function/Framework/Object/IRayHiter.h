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

        virtual Bound3f GetBoundingBox() const = 0;
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const = 0;
        virtual bool IntersectTest(const Ray3f& ray, float tMin = 0,
            float tMax = std::numeric_limits<float>::infinity()) const = 0;
    };
}