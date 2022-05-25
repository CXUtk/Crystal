#include "Brute.h"

namespace tracer
{
    Brute::Brute()
    {}

    Brute::~Brute()
    {}

    void Brute::Build(const std::vector<std::shared_ptr<IRayPrimitive>>& primitive)
    {
        for (auto& ptr : primitive)
        {
            m_primitives.push_back(cptr(ptr));
            m_masterBox = m_masterBox.Union(ptr->GetBoundingBox());
        }
    }

    bool Brute::Intersect(const crystal::Ray3f& ray, SurfaceInteraction* isec,
        float tMin, float tMax) const
    {
        int cnt = 0;
        RayHitPayload payload;
        for (auto& obj : m_primitives)
        {
            Float tCurrent = std::numeric_limits<Float>::infinity();
            if (obj->IntersectTest(ray, &tCurrent, tMin, tMax))
            {
                if (tCurrent < payload.Distance)
                {
                    payload.Distance = tCurrent;
                    payload.HitPrimitive = obj;
                }
            }
        }

        if (payload.HitPrimitive)
        {
            payload.HitPrimitive->Intersect(ray, isec);
        }
        return payload.HitPrimitive;
    }


    bool Brute::IntersectTest(const crystal::Ray3f& ray, const IRayPrimitive* ignoreItem,
        float tMin, float tMax) const
    {
        Float t;
        for (auto& obj : m_primitives)
        {
            if (obj == ignoreItem) continue;
            if (obj->IntersectTest(ray, &t, tMin, tMax))
            {
                return true;
            }
        }
        return false;
    }

    GPUDataPackage Brute::GetGPUData() const
    {
        GPUDataPackage package;
        for (auto& p : m_primitives)
        {
            package.AddObject(p);
        }
        package.AddObjectsNode(m_masterBox, m_primitives);
        return package;
    }
}