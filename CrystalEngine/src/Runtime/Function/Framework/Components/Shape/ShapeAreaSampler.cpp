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

    InteractionGeometryInfo ShapeAreaSampler::SampleSurfaceArea(const Vector2f& sample) const
    {
        return m_pShape->SampleSurfaceArea(sample);
    }

    InteractionGeometryInfo ShapeAreaSampler::SampleSurfaceLight(const InteractionGeometryInfo& ref, const Vector2f& sample) const
    {
        return m_pShape->SampleSurfaceLight(sample, ref);
    }

    Float ShapeAreaSampler::PdfArea(const InteractionGeometryInfo& surface_w) const
    {
        return m_pShape->PdfArea(surface_w);
    }

    Float ShapeAreaSampler::PdfLight(const InteractionGeometryInfo& ref, const Vector3f& wi) const
    {
        return m_pShape->PdfLight(ref, wi);
    }

    bool ShapeAreaSampler::Intersect(const Ray3f& ray, SurfaceInteraction* info) const
    {
        return m_pShape->Intersect(ray, info);
    }
}
