#include "CubemapEnvironmentLight.h"

namespace crystal
{
    CubemapEnvironmentLight::CubemapEnvironmentLight(const Transform& transform,
        std::shared_ptr<CPUTextureCubemap> cubemap)
        : Light(transform, LightFlags::Infinite, 1), m_cubeMap(cubemap)
    {}

    CubemapEnvironmentLight::~CubemapEnvironmentLight()
    {}

    Spectrum CubemapEnvironmentLight::Sample_Li(const SurfaceInfo & surface, const Vector2f & sample, Point3f * endpoint, float* pdf) const
    {
        auto& dir = m_cubeMap->WeightedSampleUV(sample);
        *pdf = m_cubeMap->Pdf(dir) / 6.f;
        *endpoint = surface.GetPosition() + dir * 10000.f;
        auto l =  m_cubeMap->Sample(dir);
        return l;
    }

    float CubemapEnvironmentLight::Pdf_Li(const SurfaceInfo& surface, const Vector3f& wi) const
    {
        return m_cubeMap->Pdf(wi);
    }

    Spectrum CubemapEnvironmentLight::Le(const Vector3f& wi) const
    {
        return m_cubeMap->Sample(wi);
    }

    Spectrum CubemapEnvironmentLight::Flux() const
    {
        return Spectrum(m_cubeMap->AverageWeights());
    }
}
