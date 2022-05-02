#include "ShapeComponent.h"
#include "ShapeRayHiter.h"
#include "ShapeAreaSampler.h"

#include "Shapes/Sphere.h"
#include "Shapes/Triangle.h"
#include <Function/Framework/Components/Transform/TransformComponent.h>

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

    std::shared_ptr<IRayHiter> ShapeComponent::GetRayHiter() const
    {
        return std::make_shared<ShapeRayHiter>(m_attachedObject, cptr(m_shape));
    }

    std::shared_ptr<IAreaSampler> ShapeComponent::GetAreaSampler() const
    {
        return std::make_shared<ShapeAreaSampler>(cptr(m_shape));
    }
}
