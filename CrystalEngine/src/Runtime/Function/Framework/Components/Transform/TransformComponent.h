#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include <Core/Math/Transform.h>

namespace crystal
{
    class TransformComponent : public Component
    {
    public:
        TransformComponent(GameObject* attachedObject);
        virtual ~TransformComponent() override;

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

        const Transform& GetTransform() const { return m_transform; }
        Transform& GetTransform() { return m_transform; }
    private:
        Transform   m_transform{};
    };
}