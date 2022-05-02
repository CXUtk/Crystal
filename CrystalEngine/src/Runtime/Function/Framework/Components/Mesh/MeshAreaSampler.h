#pragma once
#include <Function/Framework/Components/Shape/ShapeAreaSampler.h>
namespace crystal
{
    class Triangle;

    class MeshAreaSampler : public IAreaSampler
    {
    public:
        MeshAreaSampler(const std::vector<std::shared_ptr<Triangle>>* triangles,
            const std::vector<Float>* weights);
        virtual ~MeshAreaSampler() {};

        virtual Float SurfaceArea() const override;
        virtual SurfaceInfo SampleSurface(const Vector2f& sample) const override;
        virtual Float SurfacePdf(const SurfaceInfo& surface_w) const override;
        virtual Float PdfLight(const SurfaceInfo& surface_w, const Vector3f& wi) const override;

    private:
        const std::vector<std::shared_ptr<Triangle>>*  m_triangles{};
        const std::vector<Float>*                      m_weightPrefixSum{};
    };
}