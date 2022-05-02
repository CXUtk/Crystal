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

        const ShapeComponent* shapeComp = nullptr;
        if (m_attachedObject->HasComponent<ShapeComponent>())
        {
            shapeComp = m_attachedObject->GetComponent<ShapeComponent>().get();
        }

        LightType type = SRefl::EnumInfo<crystal::LightType>::string_to_enum(m_setting["Type"].Get<std::string>());
        auto& params = m_setting["Data"];
        switch (type)
        {
        case crystal::LightType::PointLight:
        {
            PointLightSettings setting = SJson::de_serialize<PointLightSettings>(params);
            m_light = std::make_shared<PointLight>(transform, setting.Intensity);
            break;
        }
        case crystal::LightType::DiffuseAreaLight:
        {
            DiffuseAreaLightSettings setting = SJson::de_serialize<DiffuseAreaLightSettings>(params);
            m_light = std::make_shared<DiffusedAreaLight>(transform,
                shapeComp->GetAreaSampler(), setting);
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

    Spectrum LightComponent::Sample_Li(const SurfaceInfo & surface_w, const Vector2f & sample, Point3f * endpoint, float* pdf) const
    {
        return m_light->Sample_Li(surface_w, sample, endpoint, pdf);
    }
}
