#include "Glass.h"
#include "../BSDFs/FresnelSpecular.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    Glass::Glass(const Spectrum& R, const Spectrum& T, Float IOR)
        : m_R(R), m_T(T), m_ior(IOR)
    {}
    Glass::~Glass()
    {}
    void Glass::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
        Float etaA = 1.0f, etaB = m_ior;
        if (!isec->IsFrontFace())
        {
            std::swap(etaA, etaB);
        }
        isec->GetBSDF()->AddBxDF(std::make_shared<FresnelSpecular>(m_R, m_T, etaA, etaB));
    }
}
