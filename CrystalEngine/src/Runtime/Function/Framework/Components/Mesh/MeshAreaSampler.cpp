#include "MeshAreaSampler.h"
#include <Function/Framework/Components/Shape/Shapes/Triangle.h>

namespace crystal
{
    MeshAreaSampler::MeshAreaSampler(const std::vector<std::shared_ptr<Triangle>>* triangles,
            const std::vector<Float>* weights)
        : m_triangles(triangles), m_weightPrefixSum(weights)
    {}

    Float MeshAreaSampler::SurfaceArea() const
    {
        return m_weightPrefixSum->back();
    }

    SurfaceInfo MeshAreaSampler::SampleSurface(const Vector2f& sample) const
    {
        Float totalArea = m_weightPrefixSum->back();
        Float w = sample.x * totalArea;
        return SurfaceInfo();
    }
}
