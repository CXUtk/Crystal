#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include "Material.h"

namespace crystal
{
    class MaterialComponent : public Component
    {
    public:
        MaterialComponent(std::shared_ptr<Material> material);
        virtual ~MaterialComponent() override;

        virtual void Initialize() override;
        virtual void Update(const GameTimer& gameTimer)override;
        virtual void Draw(const GameTimer& gameTimer)override;

        
        std::shared_ptr<Material> GetMaterial() const { return m_material; }
    private:
        std::shared_ptr<Material>  m_material = nullptr;
    };
}