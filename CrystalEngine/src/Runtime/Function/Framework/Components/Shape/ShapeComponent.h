#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include "Shapes/Shape.h"
#include <Function/Framework/Object/IRayHiter.h>

namespace crystal
{
    class ShapeComponent : public Component
    {
    public:
        ShapeComponent(std::shared_ptr<Shape> shape);
        virtual ~ShapeComponent() override;

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

        std::shared_ptr<Shape> GetShape() const { return m_shape; }
        std::shared_ptr<IRayHiter> GetRayHiter() const;

    private:
        std::shared_ptr<Shape>   m_shape{};
    };
}