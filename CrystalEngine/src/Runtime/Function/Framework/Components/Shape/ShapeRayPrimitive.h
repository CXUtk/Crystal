#pragma once
#include <Function/Framework/Interfaces/IRayPrimitive.h>
#include <Core/Math/Geometry.h>
#include <Function/Framework/Components/Medium/Medium.h>

namespace crystal
{
    class Shape;

    class ShapeRayPrimitive : public IRayPrimitive
    {
    public:
        ShapeRayPrimitive(const Shape* shape, const AreaLight* light,
            const Material* material, const MediumInterface& mediumInterface);
        virtual ~ShapeRayPrimitive() {};

        virtual const Shape* GetShape() const override { return m_pShape; }
        virtual const AreaLight* GetAreaLight() const override { return m_pAreaLight; }
        virtual const Material* GetMaterial() const override { return m_pMaterial; }
        virtual const MediumInterface& GetMediumInterface() const override { return m_mediumInterface; }

        virtual Bound3f GetBoundingBox() const;
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const;
        virtual bool IntersectTest(const Ray3f& ray, Float* t, float tMin = 0,
            float tMax = std::numeric_limits<float>::infinity()) const;

    private:
        const AreaLight*    m_pAreaLight = nullptr;
        const Shape*        m_pShape = nullptr;
        const Material*     m_pMaterial = nullptr;
        MediumInterface     m_mediumInterface{};
        Bound3f             m_boundingBox{};
    };
}