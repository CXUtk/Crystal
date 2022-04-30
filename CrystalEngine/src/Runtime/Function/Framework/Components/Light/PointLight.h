#pragma once
#include "Light.h"
#include <Core/Math/Transform.h>

namespace crystal
{
    class PointLight : public Light
    {
    public:
        PointLight(const Spectrum& intensity);
        ~PointLight() override;

        Spectrum Sample_Li(const SurfaceInfo& surface, const Transform& transform, const Vector2f& sample,
            Point3f* endpoint, float* pdf) const override;
        float Pdf_Li(const SurfaceInfo& surface, const Vector3f& wi) const override;
        Spectrum Flux() const override;

    private:
        Spectrum    m_intensity{};
    };
}