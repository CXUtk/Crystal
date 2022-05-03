#include "Scene.h"
#include <Function/Framework/Object/GameObject.h>
#include <SJson/SJson.hpp>

namespace crystal
{
    std::shared_ptr<Scene> Scene::LoadScene(const std::string& content)
    {
        auto scene = std::make_shared<Scene>();
        SJson::JsonNode jsonNode = SJson::JsonConvert::Parse(content);
        auto& objects = jsonNode["Objects"];
        objects.foreach([&scene](const SJson::JsonNode& node) {
            scene->AddObject(GameObject::LoadGameObject(node));
        });
        return scene;
    }

    Scene::Scene()
    {}

    Scene::~Scene()
    {}

    void Scene::AddObject(std::shared_ptr<GameObject> gameObject)
    {
        m_gameObjects.push_back(gameObject);
        if (gameObject->HasComponent<CameraComponent>())
        {
            m_cameras.push_back(gameObject->GetComponent<CameraComponent>().get());
        }
    }

    void Scene::Initialize()
    {
        for (auto& object : m_gameObjects)
        {
            object->Initialize();
        }
    }
}
