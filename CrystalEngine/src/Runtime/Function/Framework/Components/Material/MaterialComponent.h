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
        MaterialComponent();
        virtual ~MaterialComponent() override;

        virtual void Update(const GameTimer& gameTimer)override;
        virtual void Draw(const GameTimer& gameTimer)override;

        
        std::shared_ptr<Material> GetMaterial() const { return m_material; }
    private:
        std::shared_ptr<Material>  m_material = nullptr;
    };
}