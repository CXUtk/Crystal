#pragma once
#include "../Material.h"
#include <glm/glm.hpp>

namespace crystal
{
    class GlassMaterial : public Material
    {
    public:
        GlassMaterial(std::shared_ptr<CPUTexture2D> Ks, std::shared_ptr<CPUTexture2D> Tf, Float IOR);
        ~GlassMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        std::shared_ptr<CPUTexture2D> m_Ks{}, m_Tf{};
        Float m_ior{};
    };
}
