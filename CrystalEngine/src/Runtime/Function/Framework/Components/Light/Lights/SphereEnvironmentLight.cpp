#include "SphereEnvironmentLight.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    SphereEnvironmentLight::SphereEnvironmentLight(const Transform& transform,
        std::shared_ptr<CPUTexture2D> sphereTexture)
        : Light(transform, LightFlags::Infinite, 1), m_sphereTexture(sphereTexture)
    {}

    SphereEnvironmentLight::~SphereEnvironmentLight()
    {}

    Spectrum SphereEnvironmentLight::Sample_Li(const SurfaceInfo& surface, const Vector2f& sample, Point3f* endpoint, float* pdf) const
    {
        auto uv = m_sphereTexture->WeightedSampleUV(sample);
        float theta = (1.0 - uv.y) * glm::pi<float>();
        float phi = uv.x * glm::two_pi<float>();
        auto dir = GetUnitVector(theta, phi);

        *pdf = m_sphereTexture->Pdf(uv) / (2 * glm::pi<float>() * glm::pi<float>() * std::sin(theta));
        *endpoint = surface.GetPosition() + dir * 10000.f;

        return m_sphereTexture->Sample(uv);
    }

    float SphereEnvironmentLight::Pdf_Li(const SurfaceInfo& surface, const Vector3f& wi) const
    {
        auto thetaPhi = GetThetaPhi(wi);
        float v = thetaPhi.x / glm::pi<float>();
        float u = thetaPhi.y / glm::two_pi<float>();
        Float pdf = m_sphereTexture->Pdf(Vector2f(u, v)) / (2 * glm::pi<float>() * glm::pi<float>() * std::sin(thetaPhi.x));
        return pdf;
    }

    Spectrum SphereEnvironmentLight::Le(const Vector3f& wi) const
    {
        auto thetaPhi = GetThetaPhi(wi);
        float v = thetaPhi.x / glm::pi<float>();
        float u = thetaPhi.y / glm::two_pi<float>();
        return m_sphereTexture->Sample(Vector2f(u, v));
    }

    Spectrum SphereEnvironmentLight::Flux() const
    {
        return Spectrum(m_sphereTexture->AverageWeights());
    }
}
