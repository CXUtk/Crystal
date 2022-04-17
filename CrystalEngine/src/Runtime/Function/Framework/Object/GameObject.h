#pragma once
#include <string>
#include <vector>
#include <memory>

#include <Function/Framework/Framework.h>
#include <Core/Utils/GameTimer.h>

namespace crystal
{
    class GameObject
    {
    public:
        GameObject();
        virtual ~GameObject();

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

    private:
        std::string     m_name;
        std::vector<std::shared_ptr<Component>>     m_components;
    };
}