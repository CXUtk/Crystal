#include "TransformComponent.h"

namespace crystal
{
    TransformComponent::TransformComponent(GameObject* attachedObject)
        : Component(attachedObject)
    {}

    TransformComponent::~TransformComponent()
    {}

    void TransformComponent::Update(const GameTimer & gameTimer)
    {}

    void TransformComponent::Draw(const GameTimer & gameTimer)
    {}
}
