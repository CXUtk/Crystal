#pragma once
#include "../BSDF.h"
#include "Function/Framework/Components/Material/Models/Fresnel.h"

namespace crystal
{
    class FresnelSpecular : public BxDF
    {
    public:
        FresnelSpecular(const Spectrum& R, const Spectrum& T, Float etaA, Float etaB);
        ~FresnelSpecular() override;

        Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const override;

        float Pdf(const Vector3f& wOut, const Vector3f& wIn) const override;
        Spectrum SampleDirection(const Vector2f& sample, const Vector3f& wOut,
            Vector3f* wIn, float* pdf, BxDFType* sampledType) const override;

        Spectrum CalculateBSDFNoLDivideByPdf(const Vector3f& wOut,
            const Vector3f& wIn, BxDFType scatterType) const override;

    private:
        Spectrum m_R{}, m_T{};
        std::shared_ptr<FresnelDielectric> m_F{};
        Float m_etaA{}, m_etaB{};
    };
}