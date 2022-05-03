#include "GlassMaterial.h"
#include "../BSDFs/FresnelSpecular.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    GlassMaterial::GlassMaterial(std::shared_ptr<CPUTexture2D> Ks,
        std::shared_ptr<CPUTexture2D> Tf, Float IOR)
        : m_Ks(Ks), m_Tf(Tf), m_ior(IOR)
    {}

    GlassMaterial::~GlassMaterial()
    {}

    void GlassMaterial::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
        Float etaA = 1.0f, etaB = m_ior;
        if (!isec->IsFrontFace())
        {
            std::swap(etaA, etaB);
        }
        auto R = m_Ks->Sample(isec->GetTexCoord());
        auto T = m_Tf->Sample(isec->GetTexCoord());
        isec->GetBSDF()->AddBxDF(std::make_shared<FresnelSpecular>(R, T, etaA, etaB));
    }
}
