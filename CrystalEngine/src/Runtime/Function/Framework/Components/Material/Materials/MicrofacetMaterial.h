#pragma once
#include "../Material.h"
#include <glm/glm.hpp>
#include <Function/Framework/Components/Material/Models/Fresnel.h>

namespace crystal
{
    class MicrofacetMaterial : public Material
    {
    public:
        MicrofacetMaterial(std::shared_ptr<CPUTexture2D> R,
            std::shared_ptr<CPUTexture2D> roughness, Float IOR);
        ~MicrofacetMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        std::shared_ptr<CPUTexture2D> m_R{};
        std::shared_ptr<CPUTexture2D> m_roughness{};
        std::shared_ptr<FresnelDielectric>  m_F{};
    };
}
