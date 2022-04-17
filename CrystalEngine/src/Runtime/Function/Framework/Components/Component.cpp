#include "Component.h"

namespace crystal
{
    Component::Component(GameObject* attachedObject)
        : m_attachedObject(attachedObject)
    {}

    Component::~Component()
    {
        m_attachedObject = nullptr;
    }

    void Component::Update(const GameTimer& gameTimer)
    {}

    void Component::Draw(const GameTimer & gameTimer)
    {}
}
