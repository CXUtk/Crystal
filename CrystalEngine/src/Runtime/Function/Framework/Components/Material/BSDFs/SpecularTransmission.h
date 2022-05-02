#pragma once
#include "../BSDF.h"
#include "Function/Framework/Components/Material/Models/Fresnel.h"

namespace crystal
{
    class SpecularTransmission : public BxDF
    {
    public:
        SpecularTransmission(const Spectrum& T, std::shared_ptr<Fresnel> fresnel);
        ~SpecularTransmission() override;

        Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const override;

        float Pdf(const Vector3f& wOut, const Vector3f& wIn) const override;
        Spectrum SampleDirection(const Vector2f& sample, const Vector3f& wOut,
            Vector3f* wIn, float* pdf, BxDFType* sampledType) const override;

    private:
        Spectrum                    m_T{};
        std::shared_ptr<Fresnel>    m_fresnel{};
    };
}