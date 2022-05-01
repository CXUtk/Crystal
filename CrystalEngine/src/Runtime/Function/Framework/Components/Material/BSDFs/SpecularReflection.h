#pragma once
#include "../BSDF.h"
#include "Function/Framework/Components/Material/Models/Fresnel.h"

namespace crystal
{
    class SpecularReflection : public BxDF
    {
    public:
        SpecularReflection(const Spectrum& albedo, std::shared_ptr<Fresnel> fresnel);
        ~SpecularReflection() override;

        Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const override;

        float Pdf(const Vector3f& wOut, const Vector3f& wIn) const override;
        Spectrum SampleDirection(const Vector2f& sample, const Vector3f& wOut,
            Vector3f* wIn, float* pdf, BxDFType* sampledType) const override;

    private:
        Spectrum                    m_albedo{};
        std::shared_ptr<Fresnel>    m_fresnel{};
    };
}