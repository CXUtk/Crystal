#include "SphereEnvironmentLight.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    SphereEnvironmentLight::SphereEnvironmentLight(const Transform& transform,
        std::shared_ptr<CPUTexture2D> sphereTexture)
        : Light(transform, LightFlags::Infinite, 1), m_sphereTexture(sphereTexture)
    {
        m_distribution = m_sphereTexture->GetDistributionSampler(DistributionMapType::ThetaPhi);
    }

    SphereEnvironmentLight::~SphereEnvironmentLight()
    {}

    void SphereEnvironmentLight::Preprocess(const Bound3f& worldBound)
    {
        Float worldR = glm::length(worldBound.GetMaxPos() - worldBound.GetMinPos()) / 2;
        m_flux = Spectrum(m_distribution->FuncInt() * 2 * glm::pi<float>() * worldR * worldR);
    }

    Spectrum SphereEnvironmentLight::Sample_Li(const SurfaceGeometryInfo& surface, const Vector2f& sample, Point3f* endpoint, float* pdf) const
    {
        Float mapPdf;
        auto uv = m_distribution->SampleContinuous(sample, &mapPdf);

        //m_sphereTexture->WeightedSampleUV(sample);
        float theta = uv.y * glm::pi<float>();
        float phi = uv.x * glm::two_pi<float>();
        auto dir = GetUnitVector(theta, phi);

        *pdf = mapPdf / (2 * glm::pi<float>() * glm::pi<float>() * std::sin(theta));
        *endpoint = surface.GetPosition() + dir * 10000.f;

        uv.y = 1.0f - uv.y;
        return m_sphereTexture->Sample(uv);
    }

    float SphereEnvironmentLight::Pdf_Li(const SurfaceGeometryInfo& surface, const Vector3f& wi) const
    {
        auto thetaPhi = GetThetaPhi(wi);
        float v = thetaPhi.x / glm::pi<float>();
        float u = thetaPhi.y / glm::two_pi<float>();
        return m_distribution->ContinuousPdf(Vector2f(u, v))
            / (2 * glm::pi<float>() * glm::pi<float>() * std::sin(thetaPhi.x));
    }

    Spectrum SphereEnvironmentLight::Le(const Vector3f& wi) const
    {
        auto thetaPhi = GetThetaPhi(wi);
        float v = thetaPhi.x / glm::pi<float>();
        float u = thetaPhi.y / glm::two_pi<float>();
        return m_sphereTexture->Sample(Vector2f(u, 1.0f - v));
    }

    Spectrum SphereEnvironmentLight::Flux() const
    {
        return m_flux;
    }
}
