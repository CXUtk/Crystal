#include "CameraComponent.h"
#include <Function/Framework/Object/GameObject.h>
#include <Function/Framework/Components/Transform/TransformComponent.h>

namespace crystal
{
    CameraComponent::CameraComponent(std::shared_ptr<Camera> camera)
        : m_camera(camera)
    {

    }

    CameraComponent::~CameraComponent()
    {}

    void CameraComponent::Update(const GameTimer & gameTimer)
    {}

    void CameraComponent::Draw(const GameTimer & gameTimer)
    {}

    Ray3f CameraComponent::GenerateRay(const Vector2f & coord) const
    {
        auto transComp = m_attachedObject->GetComponent<TransformComponent>();
        auto dir = m_camera->GetDirection(coord, transComp->GetTransform());
        return Ray3f(transComp->GetTransform().GetTranslation(), dir);
    }
}
