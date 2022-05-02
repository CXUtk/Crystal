#include "DiffusedAreaLight.h"

namespace crystal
{
    DiffusedAreaLight::DiffusedAreaLight(const Transform& transform, std::shared_ptr<IAreaSampler> areaSampler,
        const DiffuseAreaLightSettings& setting)
        : AreaLight(transform, setting.NumSamples, areaSampler), m_Le(setting.LEmit)
    {
        m_totalArea = m_areaSampler->SurfaceArea();
    }

    DiffusedAreaLight::~DiffusedAreaLight()
    {}

    Spectrum DiffusedAreaLight::Flux() const
    {
        return m_totalArea * glm::pi<float>() * m_Le;
    }

    Spectrum DiffusedAreaLight::Sample_Li(const SurfaceInfo& surface_w,
        const Vector2f& sample, Point3f* endpoint, float* pdf) const
    {
        auto surface_light = m_areaSampler->SampleSurface(sample);
        *endpoint = surface_light.GetPosition();

        auto dir = *endpoint - surface_w.GetPosition();
        *pdf = m_areaSampler->PdfLight(surface_w, dir);

        return Eval_Le(surface_light, -dir);
    }

    float DiffusedAreaLight::Pdf_Li(const SurfaceInfo& surface_w, const Vector3f& wi) const
    {
        return m_areaSampler->PdfLight(surface_w, wi);
    }

    Spectrum DiffusedAreaLight::Eval_Le(const SurfaceInfo & surface, const Vector3f & wi) const
    {
        return glm::dot(surface.GetNormal(), wi) < 0 ? Spectrum(0.f) : m_Le;
    }
}
