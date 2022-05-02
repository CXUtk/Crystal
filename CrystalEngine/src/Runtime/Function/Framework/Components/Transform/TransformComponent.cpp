#include "TransformComponent.h"

namespace crystal
{
    TransformComponent::TransformComponent()
    {}

    TransformComponent::TransformComponent(const Transform& transform)
        : m_transform(transform)
    {
    }

    TransformComponent::~TransformComponent()
    {}

    void TransformComponent::Initialize()
    {}

    void TransformComponent::Update(const GameTimer & gameTimer)
    {}

    void TransformComponent::Draw(const GameTimer & gameTimer)
    {}
}
