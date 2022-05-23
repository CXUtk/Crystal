#include "HenyeyGreenstein.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    HenyeyGreenstein::~HenyeyGreenstein()
    {}

    Spectrum PhaseFunc(const Vector3f& wo, const Vector3f& wi, Float g)
    {
        Float cosTheta = wi.y;
        Float denom = 1 + g * g + 2 * g * cosTheta;
        return Spectrum((1 - g * g) / (4 * glm::pi<Float>() * denom * std::sqrt(denom)));
    }

    Spectrum HenyeyGreenstein::Pdf(const Vector3f& wo, const Vector3f& wi) const
    {
        return PhaseFunc(wo, wi, m_g);
    }

    Spectrum HenyeyGreenstein::Sample_p(const Vector3f& wo, Vector3f * wi, const Point2f & sample) const
    {
        Float cosTheta;
        if (std::abs(m_g) < 1e-3)
        {
            cosTheta = 1.f - 2 * sample.x;
        }
        else
        {
            Float c = (1 - m_g * m_g) / (1 - m_g + 2 * m_g * sample.x);
            cosTheta = (1 + m_g * m_g - c * c) / (2 * m_g);
        }
        Float phi = sample.y * glm::two_pi<float>();
        *wi = GetUnitVectorUsingCos(cosTheta, phi);
        return PhaseFunc(Vector3f(0, -1, 0), *wi, m_g);
    }
}