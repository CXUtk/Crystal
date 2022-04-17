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
        for (auto& component : m_components)
        {
            component->Update(gameTimer);
        }
    }

    void GameObject::Draw(const GameTimer& gameTimer)
    {
        for (auto& component : m_components)
        {
            component->Draw(gameTimer);
        }
    }
}
