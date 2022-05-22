#include "ShapeComponent.h"
#include "ShapeAreaSampler.h"

#include "Shapes/Sphere.h"
#include "Shapes/Triangle.h"

#include <Function/Framework/Components/Transform/TransformComponent.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>
#include <Function/Framework/Components/Medium/MediumComponent.h>

namespace crystal
{
    ShapeComponent::ShapeComponent(const SJson::JsonNode& setting)
        : m_setting(setting)
    {}

    ShapeComponent::~ShapeComponent()
    {}

    void ShapeComponent::Initialize()
    {
        auto transComp = m_attachedObject->GetComponent<TransformComponent>();
        auto& transform = transComp->GetTransform();

        ShapeType type = SRefl::EnumInfo<crystal::ShapeType>::string_to_enum(m_setting["Type"].Get<std::string>());
        auto& params = m_setting["Data"];
        switch (type)
        {
        case crystal::ShapeType::Sphere:
        {
            auto settings = SJson::de_serialize<SphereSettings>(params);
            m_shape = std::make_shared<Sphere>(settings, &transform);
            break;
        }
        case crystal::ShapeType::Triangle:
        {
            throw std::runtime_error("Not implemented");
            // m_shape = std::make_shared<Triangle>(radius, transform);
            break;
        }
        default:
            throw std::runtime_error("Not implemented");
            break;
        }
    }

    void ShapeComponent::Update(const GameTimer & gameTimer)
    {}

    void ShapeComponent::Draw(const GameTimer & gameTimer)
    {}

    std::shared_ptr<IRayPrimitive> ShapeComponent::GetRayPrimitive() const
    {
        const AreaLight* light = nullptr;
        const Material* material = nullptr;
        MediumInterface mediumInterface{};
        if (m_attachedObject->HasComponent<LightComponent>())
        {
            auto&& lightComp = m_attachedObject->GetComponent<LightComponent>();
            light = dynamic_cast<const AreaLight *>(lightComp->GetLights()[0].get());
        }
        if (m_attachedObject->HasComponent<MaterialComponent>())
        {
            auto&& materialComp = m_attachedObject->GetComponent<MaterialComponent>();
            material = materialComp->GetMaterial();
        }
        if (m_attachedObject->HasComponent<MediumComponent>())
        {
            auto&& medium = m_attachedObject->GetComponent<MediumComponent>();
            mediumInterface = medium->GetMediumInterface();
        }
        return std::make_shared<ShapeRayPrimitive>(cptr(m_shape), light, material, mediumInterface);
    }

    std::shared_ptr<IAreaSampler> ShapeComponent::GetAreaSampler() const
    {
        return std::make_shared<ShapeAreaSampler>(cptr(m_shape));
    }
}
