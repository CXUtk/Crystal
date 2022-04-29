#pragma once
#include "../Material.h"
#include <glm/glm.hpp>

namespace crystal
{
    class LambertianMaterial : public Material
    {
    public:
        LambertianMaterial(const Spectrum& albedo);
        ~LambertianMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        Spectrum m_albedo{};
    };
}
