#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <Function/Framework/Framework.h>
#include <Core/Utils/GameTimer.h>
#include <Core/Utils/Misc.h>


namespace crystal
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        void AddObject(std::shared_ptr<GameObject> gameObject);
        void Initialize();
        const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const { return m_gameObjects; }

    private:
        std::vector<std::shared_ptr<GameObject>> m_gameObjects;
    };

}