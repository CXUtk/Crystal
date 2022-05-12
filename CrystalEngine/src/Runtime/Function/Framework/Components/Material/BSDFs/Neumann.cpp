#include "Neumann.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    Neumann::Neumann(const Spectrum& albedo, int N)
        : BxDF(BxDFType::BxDF_GLOSSY | BxDFType::BxDF_REFLECTION), m_R(albedo), m_N(N)
    {}

    Neumann::~Neumann()
    {}

    Spectrum Neumann::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        // Phong模型在power很高的情况下容易出现数值精度炸裂
        if (wIn.y <= 0) return Spectrum(0.f);
        Vector3f R = glm::reflect(-wOut, Vector3f(0, 1, 0));
        float LdotR = std::max(0.f, glm::dot(R, wIn));
        float NdotV = std::max(0.f, wOut.y);
        float NdotL = std::max(0.f, wIn.y);
        return m_R * (float)((m_N + 2) / glm::two_pi<float>() * std::pow(LdotR, m_N) / std::max(NdotV, NdotL));
    }

    float Neumann::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        if (wIn.y <= 0) return 0.f;
        Vector3f R = glm::reflect(-wOut, Vector3f(0, 1, 0));
        float LdotR = std::max(0.f, glm::dot(R, wIn));
        return (m_N + 1) / glm::two_pi<float>() * std::pow(LdotR, m_N);
    }

    Spectrum Neumann::SampleDirection(const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn, float* pdf, BxDFType* sampledType) const
    {
        *sampledType = BxDFType::BxDF_REFLECTION | BxDFType::BxDF_GLOSSY;
        Vector3f R = glm::reflect(-wOut, Vector3f(0, 1, 0));
        Vector3f B = Vector3f(0, 0, 1);
        Vector3f T = glm::cross(R, B);

        float cosTheta = pow(sample.x, 1.f / (m_N + 1));
        float phi = glm::two_pi<float>() * sample.y;

        auto v = GetUnitVectorUsingCos(cosTheta, phi);
        *wIn = glm::normalize(v.x * T + v.y * R + v.z * B);
        *pdf = Pdf(wOut, *wIn);
        return DistributionFunction(wOut, *wIn);
    }

    Spectrum Neumann::CalculateBSDFNoLDivideByPdf(const Vector3f& wOut,
        const Vector3f& wIn, BxDFType scatterType) const
    {
        Float NdotL = std::max(0.f, wIn.y);
        Float NdotV = std::max(0.f, wOut.y);

        if (NdotL > NdotV)
        {
            return m_R * ((m_N + 2) / static_cast<Float>(m_N + 1));
        }
        else
        {
            return m_R * ((m_N + 2) / static_cast<Float>(m_N + 1)) * NdotL / NdotV;
        }
    }
}
