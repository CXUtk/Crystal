#include "MetalMaterial.h"
#include "../BSDFs/FresnelBlend.h"
#include "../BSDFs/MicrofacetReflection.h"
#include "../BSDFs/Lambertian.h"
#include "../BSDFs/AshikhminShirley.h"
#include "../BSDFs/Phong.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    MetalMaterial::MetalMaterial(std::shared_ptr<CPUTexture2D> Kd,
            std::shared_ptr<CPUTexture2D> roughness, const Spectrum& eta, const Spectrum& k)
        : m_Kd(Kd), m_roughness(roughness)
    {
        m_F = std::make_shared<FresnelConductor>(Spectrum(1.f), eta, k);
    }

    MetalMaterial::~MetalMaterial()
    {}

    void MetalMaterial::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
        auto baseColor = m_Kd->Sample(isec->GetTexCoord());
        auto roughness = m_roughness->Sample(isec->GetTexCoord());
        auto distribution = std::make_shared<GGXDistribution>(Vector2f(roughness.x, roughness.y));
        //isec->GetBSDF()->AddBxDF(std::make_shared<AshikhminShirley>(baseColor,
        //    Spectrum(24.f), distribution, m_F));
        isec->GetBSDF()->AddBxDF(std::make_shared<MicrofacetReflection>(baseColor, distribution, m_F));
    }
}
