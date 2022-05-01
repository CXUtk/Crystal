#include "LambertianMaterial.h"
#include "../BSDFs/Lambertian.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    LambertianMaterial::LambertianMaterial(std::shared_ptr<CPUTexture2D> albedo)
        : m_albedo(albedo)
    {
    }

    LambertianMaterial::~LambertianMaterial()
    {}

    void LambertianMaterial::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
        auto baseColor = m_albedo->Sample(isec->GetTexCoord());
        isec->GetBSDF()->AddBxDF(std::make_shared<Lambertian>(baseColor));
    }
}
