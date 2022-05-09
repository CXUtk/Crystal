#pragma once
#include "Light.h"
#include <Core/Math/Transform.h>
#include <Resource/Asset/Content/CPUTexture/CPUTexture2D.h>
#include <Resource/Asset/Content/CPUTexture/CPUTextureCubemap.h>

namespace crystal
{
    class SphereEnvironmentLight : public Light
    {
    public:
        SphereEnvironmentLight(const Transform& transform,
            std::shared_ptr<CPUTexture2D> sphereTexture);
        ~SphereEnvironmentLight() override;

        Spectrum Sample_Li(const SurfaceInfo& surface, const Vector2f& sample,
            Point3f* endpoint, float* pdf) const override;
        float Pdf_Li(const SurfaceInfo& surface, const Vector3f& wi) const override;
        virtual Spectrum Le(const Vector3f& wi) const override;
        Spectrum Flux() const override;

    private:
        std::shared_ptr<CPUTexture2D>  m_sphereTexture{};
    };
}