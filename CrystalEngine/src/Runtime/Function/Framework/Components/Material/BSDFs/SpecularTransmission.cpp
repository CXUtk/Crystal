#include "SpecularTransmission.h"

namespace crystal
{
    SpecularTransmission::SpecularTransmission(const Spectrum& T, std::shared_ptr<Fresnel> fresnel, Float etaO, Float etaI)
        : BxDF(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_SPECULAR),
        m_T(T), m_fresnel(fresnel), m_etaO(etaO), m_etaI(etaI)
    {}

    SpecularTransmission::~SpecularTransmission()
    {}

    Spectrum SpecularTransmission::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return Spectrum(0.f);
    }
    float SpecularTransmission::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return 0.0f;
    }
    Spectrum SpecularTransmission::SampleDirection(const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn, float* pdf, BxDFType* sampledType) const
    {
        *sampledType = BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_SPECULAR;
        *wIn = Vector3f(-wOut.x, wOut.y, -wOut.z);
        *pdf = 1.0f;

        Float cosTheta = std::max(0.f, -wOut.y);
        return m_T * (1.0f - m_fresnel->Eval(cosTheta));
    }
    Spectrum SpecularTransmission::CalculateBSDFNoLDivideByPdf(const Vector3f& wOut, const Vector3f& wIn, BxDFType scatterType) const
    {
        // Specular formula optimization
        Float cosTheta = std::max(0.f, -wOut.y);
        return m_T * (1.0f - m_fresnel->Eval(cosTheta)) * (m_etaO * m_etaO) / (m_etaI * m_etaI);
    }
}