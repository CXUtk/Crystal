#include "SpecularReflection.h"

namespace crystal
{
    SpecularReflection::SpecularReflection(const Spectrum& albedo, std::shared_ptr<Fresnel> fresnel)
        : BxDF(BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR), m_albedo(albedo), m_fresnel(fresnel)
    {}

    SpecularReflection::~SpecularReflection()
    {}

    Spectrum SpecularReflection::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return Spectrum(0.f);
    }
    float SpecularReflection::Pdf(const Vector3f& wOut, const Vector3f& wIn) const
    {
        return 0.0f;
    }
    Spectrum SpecularReflection::SampleDirection(const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn, float* pdf, BxDFType* sampledType) const
    {
        *sampledType = BxDFType::BxDF_REFLECTION | BxDFType::BxDF_SPECULAR;
        *wIn = Vector3f(-wOut.x, wOut.y, -wOut.z);
        *pdf = 1.0f;

        Float cosTheta = std::max(0.f, wOut.y);
        return m_albedo / cosTheta * m_fresnel->Eval(cosTheta);
    }
}