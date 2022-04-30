#include "ShapeComponent.h"
#include "ShapeRayHiter.h"

namespace crystal
{
    ShapeComponent::ShapeComponent(std::shared_ptr<Shape> shape)
        : m_shape(shape)
    {}

    ShapeComponent::~ShapeComponent()
    {}

    void ShapeComponent::Update(const GameTimer & gameTimer)
    {}

    void ShapeComponent::Draw(const GameTimer & gameTimer)
    {}

    std::shared_ptr<IRayHiter> ShapeComponent::GetRayHiter() const
    {
        return std::make_shared<ShapeRayHiter>(m_attachedObject, cptr(m_shape));
    }
}
