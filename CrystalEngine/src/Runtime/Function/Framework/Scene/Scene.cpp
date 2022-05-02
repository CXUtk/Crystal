#include "Scene.h"
#include <Function/Framework/Object/GameObject.h>

namespace crystal
{
    Scene::Scene()
    {}

    Scene::~Scene()
    {}

    void Scene::AddObject(std::shared_ptr<GameObject> gameObject)
    {
        m_gameObjects.push_back(gameObject);
    }

    void Scene::Initialize()
    {
        for (auto& object : m_gameObjects)
        {
            object->Initialize();
        }
    }
}
