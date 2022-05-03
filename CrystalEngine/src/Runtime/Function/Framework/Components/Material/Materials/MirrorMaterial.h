#pragma once
#include "../Material.h"
#include <glm/glm.hpp>

namespace crystal
{
    class MirrorMaterial : public Material
    {
    public:
        MirrorMaterial(std::shared_ptr<CPUTexture2D> Ks);
        ~MirrorMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        std::shared_ptr<CPUTexture2D> m_Ks{};
    };
}
