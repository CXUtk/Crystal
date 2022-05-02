#pragma once
#include <Function/Framework/Interfaces/IRayHiter.h>

#include <Core/Math/Geometry.h>


namespace crystal
{
    class Shape;

    class ShapeRayHiter : public IRayHiter
    {
    public:
        ShapeRayHiter(const GameObject* object, const Shape* shape);
        virtual ~ShapeRayHiter() {};

        virtual const GameObject* GetObject() const { return m_pObject; }

        virtual Bound3f GetBoundingBox() const;
        virtual bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const;
        virtual bool IntersectTest(const Ray3f& ray, float tMin = 0,
            float tMax = std::numeric_limits<float>::infinity()) const;

    private:
        const GameObject*   m_pObject = nullptr;
        const Shape*        m_pShape = nullptr;
        Bound3f             m_boundingBox{};
    };
}