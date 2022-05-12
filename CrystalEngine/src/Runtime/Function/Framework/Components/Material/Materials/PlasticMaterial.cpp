#include "PlasticMaterial.h"
#include "../BSDFs/FresnelBlend.h"
#include "../BSDFs/MicrofacetReflection.h"
#include "../BSDFs/Lambertian.h"
#include "../BSDFs/AshikhminShirley.h"
#include "../BSDFs/Phong.h"
#include <Core/Interaction/SurfaceInteraction.h>
#include <Function/Framework/Components/Material/BSDFs/Neumann.h>

namespace crystal
{
    PlasticMaterial::PlasticMaterial(std::shared_ptr<CPUTexture2D> Kd,
            std::shared_ptr<CPUTexture2D> roughness, Float IOR)
        : m_Kd(Kd), m_roughness(roughness)
    {
        m_F = std::make_shared<FresnelDielectric>(1.0, IOR);
    }

    PlasticMaterial::~PlasticMaterial()
    {}

    void PlasticMaterial::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
        auto baseColor = m_Kd->Sample(isec->GetTexCoord());
        auto roughness = m_roughness->Sample(isec->GetTexCoord());
        auto distribution = std::make_shared<GGXDistribution>(Vector2f(roughness.x, roughness.y));
        isec->GetBSDF()->AddBxDF(std::make_shared<AshikhminShirley>(baseColor,
            Spectrum(1.f), distribution));
        //isec->GetBSDF()->AddBxDF(std::make_shared<Neumann>(baseColor, std::pow(1.0f - roughness.x, 4) * 256));
    }
}
