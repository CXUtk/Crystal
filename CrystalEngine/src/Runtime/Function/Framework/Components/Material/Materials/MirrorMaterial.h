#pragma once
#include "../Material.h"
#include <glm/glm.hpp>

namespace crystal
{
    class MirrorMaterial : public Material
    {
    public:
        MirrorMaterial(const Spectrum& albedo);
        ~MirrorMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        Spectrum m_albedo{};
    };
}
