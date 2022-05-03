#pragma once
#include "Light.h"
#include <Core/Math/Transform.h>
#include <Function/Framework/Interfaces/IAreaSampler.h>

namespace crystal
{
    class AreaLight : public Light
    {
    public:
        AreaLight(const Transform& transform, int numSamples, std::shared_ptr<IAreaSampler> areaSampler)
            : Light(transform, LightFlags::Area, numSamples), m_areaSampler(areaSampler)
        {}

        virtual Spectrum Eval_Le(const SurfaceInfo& surface, const Vector3f& wi) const = 0;
        std::shared_ptr<const IAreaSampler> GetAreaSampler() const { return m_areaSampler; }
    protected:
        std::shared_ptr<IAreaSampler> m_areaSampler = nullptr;
    };
}