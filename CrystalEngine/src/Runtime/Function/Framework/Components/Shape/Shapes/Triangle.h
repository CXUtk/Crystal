#pragma once

#include "Shape.h"
#include <Function/Framework/Components/Mesh/Mesh.h>

namespace crystal
{
    class Triangle : public Shape
    {
    public:
        Triangle(const MeshVertexData* a, const MeshVertexData* b, const MeshVertexData* c);
        ~Triangle() override;

        Bound3f GetBoundingBox() const override;
        bool Intersect(const Ray3f& ray, SurfaceInteraction* isec) const override;
        bool IntersectTest(const Ray3f& ray, Float* t,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
        float SurfaceArea() const override;

        InteractionGeometryInfo SampleSurfaceArea(const Vector2f& sample) const override;
        // 采样目标点为半球的立体角
        InteractionGeometryInfo SampleSurfaceLight(const Vector2f& sample,
            const InteractionGeometryInfo& ref) const override;

    private:
        const MeshVertexData*   m_vertices[3];
        Vector3f                m_dpDu, m_dpDv;
        Bound3f                 m_bBox;

        void calculateDerivative();
        Vector3f sampleTriangle(const Vector2f& sample) const;
    };
}