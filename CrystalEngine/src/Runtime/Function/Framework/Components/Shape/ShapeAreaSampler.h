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
        virtual SurfaceInfo SampleSurface(const Vector2f& sample) const override;
        virtual Float SurfacePdf(const SurfaceInfo& surface_w) const override;
        virtual Float PdfLight(const SurfaceInfo& surface_w, const Vector3f& wi) const override;
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const override;

    private:
        const Shape*    m_pShape = nullptr;
    };
}