#pragma once
#include "Shape.h"
#include <memory>

namespace crystal
{
    struct SphereShapeDesc
    {
        Point3f     Position;
        float       Radius;
        Quaternion  Rotation;
    };
    class Sphere : public Shape
    {
    public:
        // static std::shared_ptr<Shape> CreateSphere(SphereShapeDesc pShapeNode);
        Sphere(const SphereShapeDesc& desc);
        ~Sphere() override;
        Bound3f GetBoundingBox() const override;
        bool Intersect(const Ray3f& ray, SurfaceInteraction* isec) const override;
        bool IntersectTest(const Ray3f& ray, float tMin = 0,
            float tMax = std::numeric_limits<float>::infinity()) const override;
        float SurfaceArea() const override;

        SurfaceInfo SampleSurface(const Vector2f& sample) const override;
        // 采样目标点为半球的立体角
        SurfaceInfo SampleSurfaceLight(const Vector2f& sample,
            const SurfaceInteraction& ref) const override;

    private:
        Point3f m_position{};
        float m_radius{};
        Quaternion m_rotation{};
        Matrix3f _world2Local{};
        Matrix3f _local2World{};
    };
}