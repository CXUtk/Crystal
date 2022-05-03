#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <Function/Framework/Framework.h>
#include <Function/Framework/Components/Camera/CameraComponent.h>
#include <Core/Utils/GameTimer.h>
#include <Core/Utils/Misc.h>


namespace crystal
{
    class Scene
    {
    public:
        static std::shared_ptr<Scene> LoadScene(const std::string& content);

        Scene();
        ~Scene();

        void AddObject(std::shared_ptr<GameObject> gameObject);
        void Initialize();
        const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const { return m_gameObjects; }
        std::vector<const CameraComponent*> GetCameraComponents() const { return m_cameras; }

    private:
        std::vector<std::shared_ptr<GameObject>> m_gameObjects{};
        std::vector<const CameraComponent*> m_cameras{};
    };

}