#pragma once
#include <string>
#include <vector>
#include <memory>

#include <Function/Framework/Framework.h>
#include <Core/Utils/GameTimer.h>

namespace crystal
{
    class Component
    {
    public:
        Component(GameObject* attachedObject);
        virtual ~Component();

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

    protected:
        GameObject* m_attachedObject = nullptr;
    };
}