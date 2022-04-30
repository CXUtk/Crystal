#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include "Light.h"

namespace crystal
{
    class LightComponent : public Component
    {
    public:
        LightComponent(std::shared_ptr<Light> light);
        virtual ~LightComponent() override;

        virtual void Update(const GameTimer& gameTimer) override;
        virtual void Draw(const GameTimer& gameTimer) override;

        std::shared_ptr<Light> GetLight() const { return m_light; }

        virtual Spectrum Flux() const { return m_light->Flux(); }

        virtual Spectrum Sample_Li(const SurfaceInfo& surface_w,
            const Vector2f& sample, Point3f* endpoint, float* pdf) const;

        virtual float Pdf_Li(const SurfaceInfo& surface_w, const Vector3f& wi) const
        { return m_light->Pdf_Li(surface_w, wi); }

        virtual Spectrum Le(const Vector3f& wi) const { return m_light->Flux(); }

    private:
        std::shared_ptr<Light>  m_light = nullptr;
    };
}