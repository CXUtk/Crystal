#include "GameObject.h"

#include <Function/Framework/Components/Component.h>

namespace crystal
{
    GameObject::GameObject()
    {}

    GameObject::~GameObject()
    {}

    void GameObject::Update(const GameTimer& gameTimer)
    {
        for (auto& entry : m_components)
        {
            entry.second->Update(gameTimer);
        }
    }

    void GameObject::Draw(const GameTimer& gameTimer)
    {
        for (auto& entry : m_components)
        {
            entry.second->Draw(gameTimer);
        }
    }
    void GameObject::AddComponent(std::shared_ptr<Component> component)
    {
        m_components[typeid(*component.get())] = component;
        component->AttachToObject(this);
    }
}
