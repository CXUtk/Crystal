#pragma once
#include "../BSDF.h"
#include "Function/Framework/Components/Material/Models/Fresnel.h"
#include <Function/Framework/Components/Material/Models/MicrofacetDistribution.h>

namespace crystal
{
    class AshikhminShirley : public BxDF
    {
    public:
        AshikhminShirley(const Spectrum& Rd, const Spectrum& Rs, std::shared_ptr<MicrofacetDistribution> microfacet);
        ~AshikhminShirley() override;

        Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const override;

        float Pdf(const Vector3f& wOut, const Vector3f& wIn) const override;
        Spectrum SampleDirection(const Vector2f& sample, const Vector3f& wOut,
            Vector3f* wIn, float* pdf, BxDFType* sampledType) const override;

    private:
        Spectrum                    m_Rd{}, m_Rs{};
        Float                       m_roughness{};

        std::shared_ptr<MicrofacetDistribution> m_microfacet{};
    };
}