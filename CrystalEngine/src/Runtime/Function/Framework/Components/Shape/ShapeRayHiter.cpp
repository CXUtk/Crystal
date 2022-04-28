#include "ShapeRayHiter.h"
#include <Function/Framework/Components/Shape/Shapes/Shape.h>

namespace crystal
{
    ShapeRayHiter::ShapeRayHiter(const GameObject* object, const Shape* shape)
        : m_pObject(object), m_pShape(shape)
    {
        m_boundingBox = shape->GetBoundingBox();
    }

    Bound3f ShapeRayHiter::GetBoundingBox() const
    {
        return m_boundingBox;
    }
    bool ShapeRayHiter::Intersect(const Ray3f& ray, SurfaceInteraction* info) const
    {
        return m_pShape->Intersect(ray, info);
    }
    bool ShapeRayHiter::IntersectTest(const Ray3f& ray, float tMin, float tMax) const
    {
        return m_pShape->IntersectTest(ray, tMin, tMax);
    }
}