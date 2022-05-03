#pragma once
#include <Function/Framework/Interfaces/IRayPrimitive.h>

#include <Core/Math/Geometry.h>


namespace crystal
{
    class Shape;

    class ShapeRayPrimitive : public IRayPrimitive
    {
    public:
        ShapeRayPrimitive(const Shape* shape, const AreaLight* light, const Material* material);
        virtual ~ShapeRayPrimitive() {};

        virtual const Shape* GetShape() const override { return m_pShape; }
        virtual const AreaLight* GetAreaLight() const override { return m_pAreaLight; }
        virtual const Material* GetMaterial() const override { return m_pMaterial; }

        virtual Bound3f GetBoundingBox() const;
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const;
        virtual bool IntersectTest(const Ray3f& ray, float tMin = 0,
            float tMax = std::numeric_limits<float>::infinity()) const;

    private:
        const AreaLight*    m_pAreaLight = nullptr;
        const Shape*        m_pShape = nullptr;
        const Material*     m_pMaterial = nullptr;
        Bound3f             m_boundingBox{};
    };
}