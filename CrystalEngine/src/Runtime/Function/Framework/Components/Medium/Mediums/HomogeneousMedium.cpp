#include "HomogeneousMedium.h"
#include "../PhaseFunctions/HenyeyGreenstein.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    crystal::HomogeneousMedium::HomogeneousMedium(const Spectrum& sigmaA,
        const Spectrum& sigmaS, Float g)
        :m_sigmaA(sigmaA), m_sigmaS(sigmaS)
    {
        m_phaseFunction = std::make_shared<HenyeyGreenstein>(g);
    }

    HomogeneousMedium::~HomogeneousMedium()
    {}

    Spectrum HomogeneousMedium::Tr(const Ray3f & ray, Float t, Sampler * sampler) const
    {
        Float d = std::min(t * glm::length(ray.Dir()), std::numeric_limits<Float>::max());
        Spectrum sigmaT = m_sigmaA + m_sigmaS;
        Spectrum absorption = glm::exp(-(d * sigmaT));
        return absorption;
    }

    Spectrum HomogeneousMedium::Sample(const Ray3f& ray, Float t, Sampler* sampler, MediumInteractionInfo* mi) const
    {
        constexpr int SpectrumChannels = 3;
        int channel = std::min((int)(sampler->Get1D() * SpectrumChannels), SpectrumChannels - 1);
        Spectrum sigmaT = m_sigmaA + m_sigmaS;

        Float dist = -std::log(1 - sampler->Get1D()) / sigmaT[channel];
        Float distance = t * glm::length(ray.Dir());

        bool sampledMedium = dist < distance;
        if (dist > distance)
        {
            dist = distance;
        }

        Spectrum tr = glm::exp(-(std::min(dist, std::numeric_limits<Float>::max()) * sigmaT));

        if (sampledMedium)
        {
            *mi = MediumInteractionInfo(ray.Eval(dist / glm::length(ray.Dir())), -glm::normalize(ray.Dir()),
                this);
        }

        Spectrum density = sampledMedium ? (sigmaT * tr) : tr;
        Float pdf = 0;
        for (int i = 0; i < SpectrumChannels; ++i)
            pdf += density[i];
        pdf *= 1.f / SpectrumChannels;
        return sampledMedium ? (tr * m_sigmaS / pdf) : (tr / pdf);
    }
}
