#pragma once
#include "IAccStructure.h"

namespace tracer
{
    class Brute : public IAccStructure
    {
    public:
        Brute();
        ~Brute() override;
        void Build(const std::vector<std::shared_ptr<IRayPrimitive>>& objects) override;
        bool Intersect(const crystal::Ray3f& ray, SurfaceInteraction* info,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
        bool IntersectTest(const crystal::Ray3f& ray, const IRayPrimitive* ignoreShape,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

        GPUDataPackage GetGPUData() const override;
    private:
        std::vector<const IRayPrimitive*> m_primitives{};
        Bound3f m_masterBox;
    };
}
