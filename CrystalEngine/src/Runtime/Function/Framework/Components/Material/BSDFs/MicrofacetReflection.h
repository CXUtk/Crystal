#pragma once
#include "../BSDF.h"
#include "Function/Framework/Components/Material/Models/Fresnel.h"
#include <Function/Framework/Components/Material/Models/MicrofacetDistribution.h>

namespace crystal
{
    class MicrofacetReflection : public BxDF
    {
    public:
        MicrofacetReflection(const Spectrum& albedo, std::shared_ptr<MicrofacetDistribution> microfacet,
            std::shared_ptr<Fresnel> fresnel);
        ~MicrofacetReflection() override;

        Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const override;

        float Pdf(const Vector3f& wOut, const Vector3f& wIn) const override;
        Spectrum SampleDirection(const Vector2f& sample, const Vector3f& wOut,
            Vector3f* wIn, float* pdf, BxDFType* sampledType) const override;

    private:
        Spectrum                    m_R{};
        Float                       m_roughness{};
        std::shared_ptr<Fresnel>    m_fresnel{};

        std::shared_ptr<MicrofacetDistribution> m_microfacet{};
    };
}