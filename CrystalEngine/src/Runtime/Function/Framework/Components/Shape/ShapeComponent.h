#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SJson/SJson.hpp>

#include "../Component.h"
#include "Shapes/Shape.h"
#include "ShapeRayPrimitive.h"

#include <Function/Framework/Interfaces/IAreaSampler.h>


namespace crystal
{
    class ShapeComponent : public Component
    {
    public:
        ShapeComponent(const SJson::JsonNode& setting);
        virtual ~ShapeComponent() override;

        virtual void Initialize() override;
        virtual void Update(const GameTimer& gameTimer)override;
        virtual void Draw(const GameTimer& gameTimer)override;

        std::shared_ptr<Shape> GetShape() const { return m_shape; }
        std::shared_ptr<IRayPrimitive> GetRayPrimitive() const;
        std::shared_ptr<IAreaSampler> GetAreaSampler() const;

    private:
        SJson::JsonNode             m_setting{};
        std::shared_ptr<Shape>      m_shape{};
    };
}

