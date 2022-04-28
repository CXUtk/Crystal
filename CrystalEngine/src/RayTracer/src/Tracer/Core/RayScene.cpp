#include "RayScene.h"
#include <Function/Framework/Components/Shape/ShapeComponent.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Mesh/MeshComponent.h>

namespace tracer
{
    RayScene::RayScene(std::shared_ptr<Scene> scene)
        : m_scene(scene)
    {
        m_accelStructure = IAccStructure::CreateAccelerator("");

        for (auto& obj : scene->GetGameObjects())
        {
            if (obj->HasComponent<ShapeComponent>())
            {
                auto shapeComp = obj->GetComponent<ShapeComponent>();
                m_hiters.push_back(shapeComp->GetRayHiter());
            }
        }

        m_accelStructure->Build(m_hiters);
    }
}
