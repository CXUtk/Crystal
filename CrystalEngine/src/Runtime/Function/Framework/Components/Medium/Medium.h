#pragma once
#include <Crystal.h>
#include <glm/glm.hpp>
#include <Core/Math/Geometry.h>
#include <Core/Math/Transform.h>
#include <Core/Sampling/Sampler/Sampler.h>
#include <Platform/RHI/Graphics/GraphicsCommon.h>

#include "PhaseFunctions/PhaseFunction.h"

namespace crystal
{
    class MediumInteractionInfo;
    class Medium
    {
    public:
        virtual ~Medium() = 0 {}

        virtual Spectrum Tr(const Ray3f& ray, Float t, Sampler* sampler) const = 0;
        virtual Spectrum Sample(const Ray3f& ray, Float t, Sampler* sampler, MediumInteractionInfo* mi) const = 0;
        virtual const PhaseFunction* GetPhaseFunction() const = 0;
    };

    struct MediumInterface
    {
        const Medium* Inside = nullptr;
        const Medium* Outside = nullptr;

        MediumInterface() : Inside(nullptr), Outside(nullptr) {}
        MediumInterface(const Medium* medium) : Inside(medium), Outside(medium) {}
        MediumInterface(const Medium* inside, const Medium* outside) : Inside(inside), Outside(outside) {}

        bool IsMediumTransition() const { return Inside != Outside; }
    };
}
