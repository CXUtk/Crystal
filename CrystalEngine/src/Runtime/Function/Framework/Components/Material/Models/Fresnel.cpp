#include "Fresnel.h"
#include <algorithm>

namespace crystal
{
    Spectrum FresnelDielectric::Eval(float cosThetaI) const
    {
        auto sinThetaI = std::sqrt(std::max(0.f, 1.f - cosThetaI * cosThetaI));
        auto sinThetaT = m_etaA / m_etaB * sinThetaI;
        if (sinThetaT >= 1.f) return Spectrum(1.f);

        auto cosThetaT = std::sqrt(std::max(0.f, 1.f - sinThetaT * sinThetaT));
        auto Rparl = ((m_etaB * cosThetaI) - (m_etaA * cosThetaT)) /
                     ((m_etaB * cosThetaI) + (m_etaA * cosThetaT));
        auto Rperp = ((m_etaA * cosThetaI) - (m_etaB * cosThetaT)) /
                     ((m_etaA * cosThetaI) + (m_etaB * cosThetaT));
        return Spectrum((Rparl * Rparl + Rperp * Rperp) / 2);
    }

    Spectrum FresnelSchlick::Eval(float cosThetaI) const
    {
        return m_f0 + (Spectrum(1.f) - m_f0) * std::pow(1.f - cosThetaI, 5.f);
    }


    Spectrum FresnelConductor::Eval(float cosThetaI) const
    {
        Spectrum eta = m_etaT / m_etaI;
        Spectrum etak = m_K / m_etaI;

        Float cosTheta2 = cosThetaI * cosThetaI;
        Float sinTheta2 = 1.f - cosTheta2;

        Spectrum t0 = eta * eta - etak * etak - sinTheta2;
        Spectrum t1 = eta * eta * etak * etak * 4.f;

        Spectrum a2b2 = glm::sqrt(t0 * t0 + t1);
        Spectrum a = glm::sqrt(0.5f * (a2b2 + t0));
        Spectrum t2 = a2b2 + cosTheta2;
        Spectrum Rs = (t2 - 2.f * a * cosThetaI)
            / (t2 + 2.f * a * cosThetaI);

        Spectrum t3 = cosTheta2 * a2b2 + sinTheta2 * sinTheta2;
        Spectrum Rp = Rs * (t3 - 2.f * a * cosThetaI * sinTheta2)
            / (t3 + 2.f * a * cosThetaI * sinTheta2);
        return (Rs + Rp) * 0.5f;
    }


}