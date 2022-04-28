#include "Component.h"

namespace crystal
{
    Component::Component()
    {}

    Component::~Component()
    {
        m_attachedObject = nullptr;
    }

    void Component::Update(const GameTimer& gameTimer)
    {}

    void Component::Draw(const GameTimer& gameTimer)
    {}
}
