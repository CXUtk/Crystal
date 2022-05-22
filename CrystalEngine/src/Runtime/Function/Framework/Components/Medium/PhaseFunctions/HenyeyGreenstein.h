#pragma once
#include "../Medium.h"
#include "PhaseFunction.h"

namespace crystal
{
    class HenyeyGreenstein : public PhaseFunction
    {
    public:
        HenyeyGreenstein(Float g) : m_g(g) {}
        virtual ~HenyeyGreenstein() override;

        virtual Spectrum Pdf(const Vector3f& wo, const Vector3f& wi) const override;
        virtual Spectrum Sample_p(const Vector3f& wo, Vector3f* wi, const Point2f& sample) const override;

    private:
        Float   m_g{};
    };
}