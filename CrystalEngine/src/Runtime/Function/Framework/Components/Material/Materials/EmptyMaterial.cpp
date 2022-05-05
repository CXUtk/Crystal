#include "EmptyMaterial.h"
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    EmptyMaterial::EmptyMaterial()
    {
    }

    EmptyMaterial::~EmptyMaterial()
    {}

    void EmptyMaterial::ComputeScatteringFunctions(SurfaceInteraction* isec, bool fromCamera) const
    {
    }
}
