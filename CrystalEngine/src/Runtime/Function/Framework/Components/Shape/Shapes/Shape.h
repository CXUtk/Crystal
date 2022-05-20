#pragma once
#include <Crystal.h>
#include <memory>
#include <Core/Math/Geometry.h>
#include <Core/Interaction/SurfaceInteraction.h>
#include <SJson/SJson.hpp>

#include "../ComponentSettings.h"

namespace crystal
{
    class Shape
    {
    public:

        virtual ~Shape() = 0 {}

        virtual Bound3f GetBoundingBox() const = 0;
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const = 0;
        virtual bool IntersectTest(const Ray3f& ray, Float* t,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;

        virtual float SurfaceArea() const = 0;
        virtual SurfaceInfo SampleSurfaceArea(const Vector2f& sample) const = 0;
        virtual SurfaceInfo SampleSurfaceLight(const Vector2f& sample,
            const SurfaceInfo& ref) const = 0;

        virtual float PdfArea(const SurfaceInfo& surface_w) const { return 1.f / SurfaceArea(); }
        virtual float PdfLight(const SurfaceInfo& ref, const Vector3f& wi) const;
    private:
    };
}