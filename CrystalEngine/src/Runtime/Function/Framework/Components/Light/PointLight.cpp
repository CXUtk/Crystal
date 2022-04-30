#include "PointLight.h"

namespace crystal
{
    PointLight::PointLight(const Spectrum& intensity)
        : Light(LightFlags::DeltaPosition, 1), m_intensity(intensity)
    {}

    PointLight::~PointLight()
    {}

    Spectrum PointLight::Sample_Li(const SurfaceInfo& surface, const Transform& transform,
        const Vector2f& sample, Point3f* endpoint, float* pdf) const
    {
        auto pos = transform.GetTranslation();
        *endpoint = pos;
        *pdf = 1.0f;
        return m_intensity / glm::length2(pos - surface.GetPosition());
    }

    float PointLight::Pdf_Li(const SurfaceInfo& surface, const Vector3f& wi) const
    {
        return 0.0f;
    }

    Spectrum PointLight::Flux() const
    {
        return m_intensity * 4.f * glm::pi<float>();
    }
}
