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
        TransformComponent();
        TransformComponent(const Transform& transform);
        virtual ~TransformComponent() override;

        virtual void Update(const GameTimer& gameTimer)override;
        virtual void Draw(const GameTimer& gameTimer)override;

        const Transform& GetTransform() const { return m_transform; }
        Transform& GetTransform() { return m_transform; }
    private:
        Transform   m_transform{};
    };
}