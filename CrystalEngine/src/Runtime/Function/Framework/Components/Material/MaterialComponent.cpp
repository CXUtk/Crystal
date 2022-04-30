#include "MaterialComponent.h"

namespace crystal
{

    MaterialComponent::MaterialComponent(std::shared_ptr<Material> material)
        : m_material(material)
    {}

    MaterialComponent::~MaterialComponent()
    {}

    void MaterialComponent::Update(const GameTimer & gameTimer)
    {}

    void MaterialComponent::Draw(const GameTimer & gameTimer)
    {}
}
