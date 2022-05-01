#pragma once
#include "../Material.h"
#include <glm/glm.hpp>

namespace crystal
{
    class Glass : public Material
    {
    public:
        Glass(const Spectrum& R, const Spectrum& T, Float IOR);
        ~Glass() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
        Spectrum m_R{}, m_T{};
        Float m_ior{};
    };
}
