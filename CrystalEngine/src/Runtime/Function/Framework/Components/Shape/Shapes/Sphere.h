#pragma once
#include "Shape.h"
#include <memory>
#include <Core/Math/Transform.h>

namespace crystal
{
    class Sphere : public Shape
    {
    public:
        // static std::shared_ptr<Shape> CreateSphere(SphereShapeDesc pShapeNode);
        Sphere(const SphereSettings& settings, const Transform* transform);
        ~Sphere() override;
        Bound3f GetBoundingBox() const override;
        bool Intersect(const Ray3f& ray, SurfaceInteraction* isec) const override;
        bool IntersectTest(const Ray3f& ray, Float* t,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
        float SurfaceArea() const override;

        SurfaceInfo SampleSurfaceArea(const Vector2f& sample) const override;
        // 采样目标点为半球的立体角
        SurfaceInfo SampleSurfaceLight(const Vector2f& sample,
            const SurfaceInfo& ref) const override;

        virtual float PdfLight(const SurfaceInfo& surface_w, const Vector3f& wi) const override;

    private:
        float       m_radius{};
        Point3f     m_position{};
        Matrix3f    m_world2Local{};
        Matrix3f    m_local2World{};
    };
}