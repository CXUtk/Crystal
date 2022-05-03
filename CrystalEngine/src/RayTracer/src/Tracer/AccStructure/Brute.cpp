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
        }
    }

    bool Brute::Intersect(const crystal::Ray3f& ray, SurfaceInteraction* isec,
        float tMin, float tMax) const
    {
        bool hit = false;
        int cnt = 0;
        for (auto& obj : m_primitives)
        {
            SurfaceInteraction tmp;
            if (obj->Intersect(ray, &tmp))
            {
                auto dist = tmp.GetDistance();
                if (dist < isec->GetDistance())
                {
                    tmp.SetHitPrimitive(obj);
                    *isec = std::move(tmp);
                }
                hit = true;
            }
        }
        return hit;
    }


    bool Brute::IntersectTest(const crystal::Ray3f& ray, const IRayPrimitive* ignoreItem,
        float tMin, float tMax) const
    {
        for (auto& obj : m_primitives)
        {
            if (obj == ignoreItem) continue;
            if (obj->IntersectTest(ray, tMin, tMax))
            {
                return true;
            }
        }
        return false;
    }
}