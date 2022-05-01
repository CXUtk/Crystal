#pragma once
#include "../Material.h"
#include <glm/glm.hpp>
#include <Function/Framework/Components/Material/Models/Fresnel.h>

namespace crystal
{
    class PlasticMaterial : public Material
    {
    public:
        PlasticMaterial(std::shared_ptr<CPUTexture2D> Kd,
            std::shared_ptr<CPUTexture2D> roughness, Float IOR);
        ~PlasticMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        std::shared_ptr<CPUTexture2D> m_Kd{};
        std::shared_ptr<CPUTexture2D> m_roughness{};
        std::shared_ptr<FresnelDielectric>  m_F{};
    };
}
