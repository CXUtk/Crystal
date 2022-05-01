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


}