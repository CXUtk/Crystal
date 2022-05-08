#pragma once
#include "../BSDF.h"
#include "Function/Framework/Components/Material/Models/Fresnel.h"
#include <Function/Framework/Components/Material/Models/MicrofacetDistribution.h>

namespace crystal
{
    class Neumann : public BxDF
    {
    public:
        Neumann(const Spectrum& albedo, int N);
        ~Neumann() override;

        Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const override;

        float Pdf(const Vector3f& wOut, const Vector3f& wIn) const override;
        Spectrum SampleDirection(const Vector2f& sample, const Vector3f& wOut,
            Vector3f* wIn, float* pdf, BxDFType* sampledType) const override;

    private:
        Spectrum    m_R{};
        int         m_N{};
    };
}