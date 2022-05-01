#include "MirrorMaterial.h"
#include "../BSDFs/SpecularReflection.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    MirrorMaterial::MirrorMaterial(const Spectrum& albedo)
        : m_albedo(albedo)
    {
    }

    MirrorMaterial::~MirrorMaterial()
    {}

    void MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
        auto F = std::make_shared<FresnelNoOp>();
        isec->GetBSDF()->AddBxDF(std::make_shared<SpecularReflection>(m_albedo, F));
    }
}
