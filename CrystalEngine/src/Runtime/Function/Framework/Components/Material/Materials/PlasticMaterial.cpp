#include "PlasticMaterial.h"
#include "../BSDFs/FresnelBlend.h"
#include "../BSDFs/MicrofacetReflection.h"
#include "../BSDFs/Lambertian.h"
#include <Core/Interaction/SurfaceInteraction.h>

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

    void PlasticMaterial::ComputeScatteringFunctions(SurfaceInteraction * isec, bool fromCamera) const
    {
        auto baseColor = m_Kd->Sample(isec->GetTexCoord());
        auto roughness = m_roughness->Sample(isec->GetTexCoord()).x;
        auto distribution = std::make_shared<GGXDistribution>(Vector2f(roughness, roughness));
        isec->GetBSDF()->AddBxDF(std::make_shared<FresnelBlend>(Spectrum(0.7f, 0.f, 0.f),
            Spectrum(1.f), distribution, m_F));
    }
}
