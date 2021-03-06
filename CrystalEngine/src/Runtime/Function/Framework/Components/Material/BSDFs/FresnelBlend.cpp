#include "FresnelBlend.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    FresnelBlend::FresnelBlend(const Spectrum& Rd, const Spectrum& Rs,
        std::shared_ptr<MicrofacetDistribution> microfacet,
        std::shared_ptr<Fresnel> fresnel)
        : BxDF(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY),
        m_Rd(Rd), m_Rs(Rs), m_microfacet(microfacet), m_fresnel(fresnel)
    {}

    FresnelBlend::~FresnelBlend()
    {}

    static Spectrum SchlickFresnel(const Spectrum& F0, Float cosTheta)
    {
        auto pow5 = [](Float v) { return (v * v) * (v * v) * v; };
        return F0 + pow5(1 - cosTheta) * (Spectrum(1.) - F0);
    }

    Spectrum FresnelBlend::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
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

        auto CookTorrence = m_Rs * D * G * F / (4 * NdotI * NdotO);
        auto Diffuse = m_Rd * (1.0f - F * m_Rs) / glm::pi<float>();

        return CookTorrence + Diffuse;
    }

    float FresnelBlend::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        if (wOut.y * wIn.y < 0) return 0.f;

        auto H = glm::normalize(wOut + wIn);
        auto VdotH = std::max(0.f, glm::dot(wOut, H));

        auto NdotL = std::max(0.f, wIn.y);
        return 0.5f * (m_microfacet->Pdf(wOut, H) / (4 * VdotH) + NdotL / glm::pi<float>());
    }

    Spectrum FresnelBlend::SampleDirection(const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn, float* pdf, BxDFType* sampledType) const
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