#pragma once
#include "../Material.h"
#include <glm/glm.hpp>

namespace crystal
{
    class EmptyMaterial : public Material
    {
    public:
        EmptyMaterial();
        ~EmptyMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera = true) const override;

    private:
    };
}
