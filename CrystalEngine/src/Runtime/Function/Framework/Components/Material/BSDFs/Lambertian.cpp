#include "Lambertian.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    Lambertian::Lambertian(const Spectrum& albedo)
        : BxDF(BxDFType::BxDF_DIFFUSE | BxDFType::BxDF_REFLECTION), m_albedo(albedo)
    {}

    Lambertian::~Lambertian()
    {}

    Spectrum Lambertian::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return m_albedo / glm::pi<float>();
    }

    float Lambertian::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return std::max(0.f, wIn.y) / glm::pi<float>();
    }

    Spectrum Lambertian::SampleDirection(const Vector2f& sample, const Vector3f& wOut,
        Vector3f* wIn, float* pdf, BxDFType* sampledType) const
    {
        *sampledType = BxDFType::BxDF_REFLECTION | BxDFType::BxDF_DIFFUSE;
        *wIn = NextCosineUnitHemiSphere(sample, pdf);
        return DistributionFunction(wOut, *wIn);
    }

    Spectrum Lambertian::CalculateBSDFNoLDivideByPdf(const Vector3f& wOut, const Vector3f& wIn, BxDFType scatterType) const
    {
        return m_albedo;
    }
}
