#pragma once
#include "AreaLight.h"
#include "../ComponentSettings.h"

namespace crystal
{
    class DiffusedAreaLight : public AreaLight
    {
    public:
        // Diffused area uses emit radiance as standard unit
        DiffusedAreaLight(const Transform& transform, std::shared_ptr<IAreaSampler> areaSampler,
            const DiffuseAreaLightSettings& setting);

        ~DiffusedAreaLight();

        virtual Spectrum Flux() const override;

        virtual Spectrum Sample_Li(const SurfaceGeometryInfo& surface_w,
            const Vector2f& sample, Point3f* endpoint, float* pdf) const override;

        virtual float Pdf_Li(const SurfaceGeometryInfo& surface_w, const Vector3f& wi) const override;

        virtual Spectrum Eval_Le(const SurfaceGeometryInfo& surface, const Vector3f& wi) const override;

        Spectrum GetLe() const { return m_Le; }

    private:
        Spectrum    m_Le{};
        Float       m_totalArea{};
    };
}