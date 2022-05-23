#pragma once
#include <Function/Framework/Interfaces/IAreaSampler.h>

#include <Core/Math/Geometry.h>

namespace crystal
{
    class Shape;

    class ShapeAreaSampler : public IAreaSampler
    {
    public:
        ShapeAreaSampler(const Shape* shape);
        virtual ~ShapeAreaSampler() {};

        virtual Float SurfaceArea() const override;

        virtual SurfaceGeometryInfo SampleSurfaceArea(const Vector2f& sample) const override;
        virtual SurfaceGeometryInfo SampleSurfaceLight(const SurfaceGeometryInfo& ref, const Vector2f& sample) const override;

        virtual Float PdfArea(const SurfaceGeometryInfo& surface_w) const override;
        virtual Float PdfLight(const SurfaceGeometryInfo& ref, const Vector3f& wi) const override;
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const override;

    private:
        const Shape*    m_pShape = nullptr;
    };
}