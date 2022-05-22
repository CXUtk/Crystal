#pragma once
#include "../Medium.h"

namespace crystal
{
    class PhaseFunction
    {
    public:
        virtual ~PhaseFunction() = 0 {};

        virtual Spectrum Pdf(const Vector3f& wo, const Vector3f& wi) const = 0;
        virtual Spectrum Sample_p(const Vector3f& wo, Vector3f* wi, const Point2f& sample) const = 0;

    private:
    };
}