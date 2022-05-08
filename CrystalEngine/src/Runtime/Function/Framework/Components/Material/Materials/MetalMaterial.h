#pragma once
#include "../Material.h"
#include <glm/glm.hpp>
#include <Function/Framework/Components/Material/Models/Fresnel.h>

namespace crystal
{
    class MetalMaterial : public Material
    {
    public:
        MetalMaterial(std::shared_ptr<CPUTexture2D> Kd,
            std::shared_ptr<CPUTexture2D> roughness, const Spectrum& eta, const Spectrum& k);
        ~MetalMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        std::shared_ptr<CPUTexture2D> m_Kd{};
        std::shared_ptr<CPUTexture2D> m_roughness{};
        std::shared_ptr<FresnelConductor>  m_F{};
    };
}
