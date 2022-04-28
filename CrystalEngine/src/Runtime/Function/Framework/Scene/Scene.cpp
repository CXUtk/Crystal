#include "Scene.h"

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
}
