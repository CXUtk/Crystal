#include "CameraComponent.h"
#include "ComponentSettings.h"

#include <Function/Framework/Object/GameObject.h>
#include <Function/Framework/Components/Transform/TransformComponent.h>

namespace crystal
{
    CameraComponent::CameraComponent(const SJson::JsonNode& setting)
        : m_setting(setting)
    {

    }

    CameraComponent::~CameraComponent()
    {}

    void CameraComponent::Initialize()
    {
        auto transComp = m_attachedObject->GetComponent<TransformComponent>();
        auto& transform = transComp->GetTransform();

        CameraType type = SRefl::EnumInfo<crystal::CameraType>::string_to_enum(m_setting["Type"].Get<std::string>());
        auto& params = m_setting["Data"];
        switch (type)
        {
        case crystal::CameraType::Perspective:
        {
            auto persp_setting = SJson::de_serialize<PerspectiveCameraSetting>(params);
            m_camera = std::make_shared<Camera>(&transform, persp_setting);
            break;
        }
        default:
            throw std::runtime_error("Not implemented");
            break;
        }
    }

    void CameraComponent::Update(const GameTimer & gameTimer)
    {}

    void CameraComponent::Draw(const GameTimer & gameTimer)
    {}

    Ray3f CameraComponent::GenerateRay(const Vector2f & coord) const
    {
        auto dir = m_camera->GetDirection(coord);
        return Ray3f(m_camera->GetEyePos(), dir);
    }
}
