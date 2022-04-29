#include "Lambertian.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    Lambertain::Lambertain(const Spectrum& albedo)
        : BxDF(BxDFType::BxDF_DIFFUSE | BxDFType::BxDF_REFLECTION), m_albedo(albedo)
    {}

    Lambertain::~Lambertain()
    {}

    Spectrum Lambertain::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return m_albedo / glm::pi<float>();
    }

    float Lambertain::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return std::max(0.f, wIn.y) / glm::pi<float>();
    }

    Spectrum Lambertain::SampleDirection(const Vector2f& sample, const Vector3f& wOut,
        Vector3f* wIn, float* pdf, BxDFType* sampledType) const
    {
        *wIn = NextCosineUnitHemiSphere(sample, pdf);
        return DistributionFunction(wOut, *wIn);
    }
}
