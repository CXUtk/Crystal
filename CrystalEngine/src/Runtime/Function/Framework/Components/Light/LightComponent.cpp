#include "LightComponent.h"
#include "ComponentSettings.h"

#include "Lights/PointLight.h"
#include "Lights/DiffusedAreaLight.h"

#include <Function/Framework/Components/Transform/TransformComponent.h>
#include <Function/Framework/Components/Mesh/MeshComponent.h>
#include <Function/Framework/Components/Shape/ShapeComponent.h>

namespace crystal
{
    LightComponent::LightComponent(const SJson::JsonNode& setting)
        : m_setting(setting)
    {}

    LightComponent::~LightComponent()
    {}

    void LightComponent::Initialize()
    {
        auto transComp = m_attachedObject->GetComponent<TransformComponent>();
        auto& transform = transComp->GetTransform();

        std::shared_ptr<ShapeComponent> shapeComp = nullptr;
        if (m_attachedObject->HasComponent<ShapeComponent>())
        {
            shapeComp = m_attachedObject->GetComponent<ShapeComponent>();
        }

        std::shared_ptr<MeshComponent> meshComp = nullptr;
        if (m_attachedObject->HasComponent<MeshComponent>())
        {
            meshComp = m_attachedObject->GetComponent<MeshComponent>();
        }

        assert((meshComp == nullptr) || (shapeComp == nullptr));


        LightType type = SRefl::EnumInfo<crystal::LightType>::string_to_enum(m_setting["Type"].Get<std::string>());
        auto& params = m_setting["Data"];
        switch (type)
        {
        case crystal::LightType::PointLight:
        {
            PointLightSettings setting = SJson::de_serialize<PointLightSettings>(params);
            m_lights.push_back(std::make_shared<PointLight>(transform, setting.Intensity));
            break;
        }
        case crystal::LightType::DiffuseAreaLight:
        {
            DiffuseAreaLightSettings setting = SJson::de_serialize<DiffuseAreaLightSettings>(params);

            if (shapeComp)
            {
                std::shared_ptr<IAreaSampler> areaSampler = shapeComp->GetAreaSampler();
                m_lights.push_back(std::make_shared<DiffusedAreaLight>(transform,
                    areaSampler, setting));
            }
            else if (meshComp)
            {
                meshComp->ForeachTriangle([&](std::shared_ptr<const Triangle> triangle) {
                    std::shared_ptr<IAreaSampler> areaSampler = std::make_shared<ShapeAreaSampler>(cptr(triangle));
                    m_lights.push_back(std::make_shared<DiffusedAreaLight>(transform,
                        areaSampler, setting));
                });
            }
            break;
        }
        default:
            throw std::runtime_error("Not implemented");
            break;
        }
    }

    void LightComponent::Update(const GameTimer & gameTimer)
    {}

    void LightComponent::Draw(const GameTimer & gameTimer)
    {}
}
