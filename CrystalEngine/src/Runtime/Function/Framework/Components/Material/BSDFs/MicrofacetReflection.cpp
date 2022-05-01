#include "MicrofacetReflection.h"

namespace crystal
{
    MicrofacetReflection::MicrofacetReflection(const Spectrum& albedo,
        std::shared_ptr<MicrofacetDistribution> microfacet,
        std::shared_ptr<Fresnel> fresnel)
        : BxDF(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY),
        m_R(albedo), m_microfacet(microfacet), m_fresnel(fresnel)
    {}

    MicrofacetReflection::~MicrofacetReflection()
    {}

    Spectrum MicrofacetReflection::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        auto NdotI = std::max(0.f, wIn.y);
        auto NdotO = std::max(0.f, wOut.y);
        if (NdotI == 0 || NdotO == 0) return Spectrum(0.f);
        auto H = glm::normalize(wOut + wIn);
        if (glm::isnan(H) != glm::bvec3(false)) return Spectrum(0.f);

        auto cosThetaI_H = std::max(0.f, glm::dot(wIn, H));

        auto D = m_microfacet->D(H);
        auto G = m_microfacet->G(wOut, wIn);
        auto F = m_fresnel->Eval(cosThetaI_H);

        return m_R * D * G * F / (4 * NdotI * NdotO);
    }

    float MicrofacetReflection::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        if (wOut.y * wIn.y < 0) return 0.f;

        auto H = glm::normalize(wOut + wIn);
        auto VdotH = std::max(0.f, glm::dot(wOut, H));
        return m_microfacet->Pdf(wOut, wIn) / (4 * VdotH);
    }

    Spectrum MicrofacetReflection::SampleDirection(const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn, float* pdf, BxDFType* sampledType) const
    {
        *sampledType = BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY;
        auto H = m_microfacet->Sample_wh(wOut, sample);
        *wIn = glm::reflect(-wOut, H);
        if (wOut.y * wIn->y < 0) return Spectrum(0.f);

        auto VdotH = std::max(0.f, glm::dot(wOut, H));
        *pdf = m_microfacet->Pdf(wOut, H) / (4 * VdotH);

        return DistributionFunction(wOut, *wIn);
    }
}