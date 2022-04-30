#include "RayScene.h"
#include <Function/Framework/Components/Shape/ShapeComponent.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Mesh/MeshComponent.h>

namespace tracer
{
    RayScene::RayScene(std::shared_ptr<Scene> scene)
        : m_scene(scene)
    {
        m_accelStructure = IAccStructure::CreateAccelerator("Brute");

        for (auto& obj : scene->GetGameObjects())
        {
            if (obj->HasComponent<ShapeComponent>())
            {
                auto shapeComp = obj->GetComponent<ShapeComponent>();
                m_hiters.push_back(shapeComp->GetRayHiter());
            }
            else if (obj->HasComponent<MeshComponent>())
            {
                auto meshComp = obj->GetComponent<MeshComponent>();
                for (auto& triangle : meshComp->CreateRayHiters())
                {
                    m_hiters.push_back(triangle);
                }
            }

            if (obj->HasComponent<LightComponent>())
            {
                m_light.push_back(obj->GetComponent<LightComponent>());
            }
        }

        m_accelStructure->Build(m_hiters);
    }

    bool RayScene::Intersect(const Ray3f& ray, SurfaceInteraction* info) const
    {
        return m_accelStructure->Intersect(ray, info);
    }

    bool RayScene::IntersectTest(const Ray3f& ray, float tMin, float tMax) const
    {
        return m_accelStructure->IntersectTest(ray, nullptr, tMin, tMax);
    }

    void RayScene::ForEachLights(std::function<void(const crystal::LightComponent*)> action) const
    {
        for (auto& light : m_light)
        {
            action(cptr(light));
        }
    }
}
