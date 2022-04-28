#include "Brute.h"

namespace tracer
{
    Brute::Brute()
    {}

    Brute::~Brute()
    {}

    void Brute::Build(const std::vector<std::shared_ptr<IRayHiter>>& objects)
    {
        for (auto& ptr : objects)
        {
            m_objects.push_back(cptr(ptr));
        }
    }

    bool Brute::Intersect(const crystal::Ray3f& ray, SurfaceInteraction* isec,
        float tMin, float tMax) const
    {
        bool hit = false;
        int cnt = 0;
        for (auto& obj : m_objects)
        {
            SurfaceInteraction tmp;
            if (obj->Intersect(ray, &tmp))
            {
                auto dist = tmp.GetDistance();
                if (dist < isec->GetDistance())
                {
                    tmp.SetHitObject(obj->GetObject());
                    *isec = std::move(tmp);
                }
                hit = true;
            }
        }
        return hit;
    }


    bool Brute::IntersectTest(const crystal::Ray3f& ray, const IRayHiter* ignoreShape,
        float tMin, float tMax) const
    {
        for (auto& obj : m_objects)
        {
            if (obj == ignoreShape) continue;
            if (obj->IntersectTest(ray, tMin, tMax))
            {
                return true;
            }
        }
        return false;
    }
}