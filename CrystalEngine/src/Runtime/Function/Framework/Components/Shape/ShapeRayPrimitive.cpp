#include "ShapeRayPrimitive.h"
#include <Function/Framework/Components/Shape/Shapes/Shape.h>

namespace crystal
{
    ShapeRayPrimitive::ShapeRayPrimitive(const Shape* shape, const AreaLight* light,
        const Material* material)
        : m_pShape(shape), m_pAreaLight(light), m_pMaterial(material)
    {
        m_boundingBox = shape->GetBoundingBox();
    }

    Bound3f ShapeRayPrimitive::GetBoundingBox() const
    {
        return m_boundingBox;
    }

    bool ShapeRayPrimitive::Intersect(const Ray3f& ray, SurfaceInteraction* info) const
    {
        return m_pShape->Intersect(ray, info);
    }

    bool ShapeRayPrimitive::IntersectTest(const Ray3f& ray, Float* t, float tMin, float tMax) const
    {
        return m_pShape->IntersectTest(ray, t, tMin, tMax);
    }
}