#include "MirrorMaterial.h"
#include "../BSDFs/SpecularReflection.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    MirrorMaterial::MirrorMaterial(std::shared_ptr<CPUTexture2D> Ks)
        : m_Ks(Ks)
    {
    }

    MirrorMaterial::~MirrorMaterial()
    {}

    void MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
        auto F = std::make_shared<FresnelNoOp>();
        auto R = m_Ks->Sample(isec->GetTexCoord());
        isec->GetBSDF()->AddBxDF(std::make_shared<SpecularReflection>(R, F));
    }
}
