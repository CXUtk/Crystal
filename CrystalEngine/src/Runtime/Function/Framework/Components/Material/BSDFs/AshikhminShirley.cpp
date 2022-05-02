#include "AshikhminShirley.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    AshikhminShirley::AshikhminShirley(const Spectrum& Rd, const Spectrum& Rs,
        std::shared_ptr<MicrofacetDistribution> microfacet,
        std::shared_ptr<Fresnel> fresnel)
        : BxDF(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY),
        m_Rd(Rd), m_Rs(Rs), m_microfacet(microfacet), m_fresnel(fresnel)
    {}

    AshikhminShirley::~AshikhminShirley()
    {}

    static Spectrum SchlickFresnel(const Spectrum& F0, Float cosTheta)
    {
        auto pow5 = [](Float v) { return (v * v) * (v * v) * v; };
        return F0 + pow5(1 - cosTheta) * (Spectrum(1.) - F0);
    }

    Spectrum AshikhminShirley::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        auto pow5 = [](Float v) { return (v * v) * (v * v) * v; };

        auto NdotI = std::max(0.f, wIn.y);
        auto NdotO = std::max(0.f, wOut.y);

        auto Ks = m_Rs * Spectrum(0.04f);

        Spectrum diffuse = (28.f / (23.f * glm::pi<float>())) * m_Rd * (Spectrum(1.f) - Ks) *
            (1 - pow5(1 - .5f * NdotI)) *
            (1 - pow5(1 - .5f * NdotO));

        if (NdotI == 0 || NdotO == 0) return Spectrum(0.f);
        auto H = glm::normalize(wOut + wIn);
        if (glm::isnan(H) != glm::bvec3(false)) return Spectrum(0.f);

        auto LdotH = std::max(0.f, glm::dot(wIn, H));

        float D = m_microfacet->D(H);
        Spectrum specular = D * SchlickFresnel(Ks, LdotH)
            / (4 * LdotH * std::max(NdotI, NdotO));

        return diffuse + specular;
    }

    float AshikhminShirley::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        if (wOut.y * wIn.y < 0) return 0.f;

        auto H = glm::normalize(wOut + wIn);
        auto VdotH = std::max(0.f, glm::dot(wOut, H));

        auto NdotL = std::max(0.f, wIn.y);
        return 0.5f * (m_microfacet->Pdf(wOut, H) / (4 * VdotH) + NdotL / glm::pi<float>());
    }

    Spectrum AshikhminShirley::SampleDirection(const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn, float* pdf, BxDFType* sampledType) const
    {
        *sampledType = BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY;

        if (sample.x < 0.5)
        {
            Vector2f newSample(sample.x * 2, sample.y);
            auto H = m_microfacet->Sample_wh(wOut, newSample);
            *wIn = glm::reflect(-wOut, H);
            if (wOut.y * wIn->y < 0) return Spectrum(0.f);
        }
        else
        {
            Vector2f newSample(sample.x * 2 - 1, sample.y);
            *wIn = NextCosineUnitHemiSphere(newSample, pdf);
        }
        *pdf = Pdf(wOut, *wIn);
        return DistributionFunction(wOut, *wIn);
    }
}