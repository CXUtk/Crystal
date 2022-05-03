#pragma once
#include <string>
#include <vector>
#include <memory>

#include "ComponentSettings.h"
#include "../Component.h"
#include "Material.h" 

namespace crystal
{
    class MaterialComponent : public Component
    {
    public:
        MaterialComponent(std::shared_ptr<Material> material);
        MaterialComponent(const SJson::JsonNode& setting);
        virtual ~MaterialComponent() override;

        virtual void Initialize() override;
        virtual void Update(const GameTimer& gameTimer)override;
        virtual void Draw(const GameTimer& gameTimer)override;

        
        const Material* GetMaterial() const { return cptr(m_material); }
    private:
        SJson::JsonNode             m_setting{};
        std::shared_ptr<Material>   m_material = nullptr;
    };
}