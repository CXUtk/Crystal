#pragma once
#include "IAccStructure.h"

namespace tracer
{
    class Brute : public IAccStructure
    {
    public:
        Brute();
        ~Brute() override;
        void Build(const std::vector<std::shared_ptr<IRayHiter>>& objects) override;
        bool Intersect(const crystal::Ray3f& ray, SurfaceInteraction* info,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
        bool IntersectTest(const crystal::Ray3f& ray, const IRayHiter* ignoreShape,
            float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

    private:
        std::vector<const IRayHiter*> m_objects;
    };
}
