#include "ShapeAreaSampler.h"
#include "Shapes/Shape.h"

namespace crystal
{
    ShapeAreaSampler::ShapeAreaSampler(const Shape* shape)
        : m_pShape(shape)
    {}

    Float ShapeAreaSampler::SurfaceArea() const
    {
        return m_pShape->SurfaceArea();
    }

    SurfaceInfo ShapeAreaSampler::SampleSurface(const Vector2f& sample) const
    {
        return m_pShape->SampleSurface(sample);
    }

    Float ShapeAreaSampler::SurfacePdf(const SurfaceInfo& surface_w) const
    {
        return m_pShape->Pdf(surface_w);
    }

    Float ShapeAreaSampler::PdfLight(const SurfaceInfo& surface_w, const Vector3f& wi) const
    {
        return m_pShape->PdfLight(surface_w, wi);
    }

    bool ShapeAreaSampler::Intersect(const Ray3f& ray, SurfaceInteraction* info) const
    {
        return m_pShape->Intersect(ray, info);
    }
}
