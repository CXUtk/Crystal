#include "FresnelSpecular.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    FresnelSpecular::FresnelSpecular(const Spectrum& R, const Spectrum& T, Float etaA, Float etaB)
        : BxDF(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR),
        m_R(R), m_T(T), m_etaA(etaA), m_etaB(etaB)
    {
        m_F = std::make_shared<FresnelDielectric>(etaA, etaB);
    }

    FresnelSpecular::~FresnelSpecular()
    {}

    Spectrum FresnelSpecular::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return Spectrum(0.f);
    }

    float FresnelSpecular::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return 0.0f;
    }

    Spectrum FresnelSpecular::SampleDirection(const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn, float* pdf, BxDFType* sampledType) const
    {
        auto cosTheta = std::max(0.f, wOut.y);
        auto fr = m_F->Eval(cosTheta);

        if (sample[0] < fr.r)
        {
            *sampledType = BxDFType(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR);
            *wIn = Vector3f(-wOut.x, wOut.y, -wOut.z);
            *pdf = fr.r;
            return fr.r * m_R;
        }
        else
        {
            *sampledType = BxDFType(BxDFType::BxDF_TRANSMISSION | BxDFType::BxDF_SPECULAR);
            refract(wOut, m_etaA, m_etaB, wIn);
            *pdf = 1.f - fr.r;
            return (1.f - fr.r) * m_T;
        }
    }
}
