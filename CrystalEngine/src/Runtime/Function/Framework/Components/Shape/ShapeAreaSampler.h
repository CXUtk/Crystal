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

        virtual InteractionGeometryInfo SampleSurfaceArea(const Vector2f& sample) const override;
        virtual InteractionGeometryInfo SampleSurfaceLight(const InteractionGeometryInfo& ref, const Vector2f& sample) const override;

        virtual Float PdfArea(const InteractionGeometryInfo& surface_w) const override;
        virtual Float PdfLight(const InteractionGeometryInfo& ref, const Vector3f& wi) const override;
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const override;

    private:
        const Shape*    m_pShape = nullptr;
    };
}