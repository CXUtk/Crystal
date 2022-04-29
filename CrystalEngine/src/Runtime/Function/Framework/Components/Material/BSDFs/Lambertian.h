#pragma once
#include "../BSDF.h"

namespace crystal
{
    class Lambertain : public BxDF
    {
    public:
        Lambertain(const Spectrum& albedo);
        ~Lambertain() override;

        Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const override;

        float Pdf(const Vector3f& wOut, const Vector3f& wIn) const override;
        Spectrum SampleDirection(const Vector2f& sample, const Vector3f& wOut,
            Vector3f* wIn, float* pdf, BxDFType* sampledType) const override;

    private:
        Spectrum m_albedo;
    };
}