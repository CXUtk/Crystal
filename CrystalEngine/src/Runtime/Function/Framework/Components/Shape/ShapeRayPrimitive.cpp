#include "ShapeRayPrimitive.h"
#include <Function/Framework/Components/Shape/Shapes/Shape.h>
#include <Function/Framework/Components/Medium/Medium.h>

namespace crystal
{
    ShapeRayPrimitive::ShapeRayPrimitive(const Shape* shape, const AreaLight* light,
        const Material* material, const MediumInterface& mediumInterface)
        : m_pShape(shape), m_pAreaLight(light),
        m_pMaterial(material), m_mediumInterface(mediumInterface)
    {
        m_boundingBox = shape->GetBoundingBox();
    }

    Bound3f ShapeRayPrimitive::GetBoundingBox() const
    {
        return m_boundingBox;
    }

    bool ShapeRayPrimitive::Intersect(const Ray3f& ray, SurfaceInteraction* info) const
    {
        if (m_pShape->Intersect(ray, info))
        {
            info->SetMediumInterface(m_mediumInterface);
        }
        return false;
    }

    bool ShapeRayPrimitive::IntersectTest(const Ray3f& ray, Float* t, float tMin, float tMax) const
    {
        return m_pShape->IntersectTest(ray, t, tMin, tMax);
    }
}