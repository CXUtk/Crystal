#include "MicrofacetMaterial.h"
#include "../BSDFs/MicrofacetReflection.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    MicrofacetMaterial::MicrofacetMaterial(std::shared_ptr<CPUTexture2D> R,
        std::shared_ptr<CPUTexture2D> roughness, Float IOR)
        : m_R(R), m_roughness(roughness)
    {
        m_F = std::make_shared<FresnelDielectric>(1.0, IOR);
    }

    MicrofacetMaterial::~MicrofacetMaterial()
    {}

    void MicrofacetMaterial::ComputeScatteringFunctions(SurfaceInteraction * isec, bool fromCamera) const
    {
        auto baseColor = m_R->Sample(isec->GetTexCoord());
        auto roughness = m_roughness->Sample(isec->GetTexCoord()).x;
        auto distribution = std::make_shared<GGXDistribution>(Vector2f(roughness, roughness));
        isec->GetBSDF()->AddBxDF(std::make_shared<MicrofacetReflection>(baseColor, distribution, m_F));
    }
}
