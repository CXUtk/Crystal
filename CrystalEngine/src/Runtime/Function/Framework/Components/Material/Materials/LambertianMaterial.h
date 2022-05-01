#pragma once
#include "../Material.h"
#include <glm/glm.hpp>

namespace crystal
{
    class LambertianMaterial : public Material
    {
    public:
        LambertianMaterial(std::shared_ptr<CPUTexture2D> albedo);
        ~LambertianMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        std::shared_ptr<CPUTexture2D> m_albedo{};
    };
}
