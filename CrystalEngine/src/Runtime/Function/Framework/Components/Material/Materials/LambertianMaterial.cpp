#include "LambertianMaterial.h"
#include "../BSDFs/Lambertian.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    LambertianMaterial::LambertianMaterial(const Spectrum& albedo)
        : m_albedo(albedo)
    {
    }

    LambertianMaterial::~LambertianMaterial()
    {}

    void LambertianMaterial::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
        isec->GetBSDF()->AddBxDF(std::make_shared<Lambertain>(m_albedo));
    }
}
