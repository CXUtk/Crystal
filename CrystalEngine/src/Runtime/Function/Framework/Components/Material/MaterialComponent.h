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

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

    private:
        std::shared_ptr<Material>  m_material = nullptr;
    };
}