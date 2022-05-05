#include "MaterialComponent.h"


namespace crystal
{

    MaterialComponent::MaterialComponent(std::shared_ptr<Material> material)
        : m_material(material)
    {}

    MaterialComponent::MaterialComponent(const SJson::JsonNode& setting)
        : m_setting(setting)
    {}

    MaterialComponent::~MaterialComponent()
    {}

    void MaterialComponent::Initialize()
    {
        if (m_setting.GetType() == SJson::ValueType::Null)
        {
            return;
        }

        m_material = Material::CreateMaterial(m_setting);
    }

    void MaterialComponent::Update(const GameTimer & gameTimer)
    {}

    void MaterialComponent::Draw(const GameTimer & gameTimer)
    {}
}
