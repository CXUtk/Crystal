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
        Component();
        virtual ~Component();

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

        void AttachToObject(GameObject* gameObject) { m_attachedObject = gameObject; }
        GameObject* GetAttachedObject() const { return m_attachedObject; }

    protected:
        GameObject* m_attachedObject = nullptr;
    };
}