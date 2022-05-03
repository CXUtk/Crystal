#include "GameObject.h"

#include <queue>
#include <Function/Framework/Components/Component.h>


namespace crystal
{
    std::shared_ptr<GameObject> GameObject::LoadGameObject(const SJson::JsonNode& node)
    {
        auto name = node["Name"].Get<std::string>();
        auto& components = node["Components"];

        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
        gameObject->SetName(name);

        components.foreach([&gameObject](const SJson::JsonNode& node) {
            gameObject->AddComponent(Component::LoadComponent(node));
        });
        return gameObject;
    }

    GameObject::GameObject()
    {}

    GameObject::~GameObject()
    {}

    void GameObject::Initialize()
    {
        topologicalSort();
        for (auto& component : m_components_topological_order)
        {
            component->Initialize();
        }
    }

    void GameObject::Update(const GameTimer& gameTimer)
    {
        for (auto& component : m_components_topological_order)
        {
            component->Update(gameTimer);
        }
    }

    void GameObject::Draw(const GameTimer& gameTimer)
    {
        for (auto& component : m_components_topological_order)
        {
            component->Draw(gameTimer);
        }
    }

    void GameObject::AddComponent(std::shared_ptr<Component> component)
    {
        m_components[typeid(*component.get())] = component;
        component->AttachToObject(this);
    }

    void GameObject::topologicalSort()
    {
        auto dependencyGraph = Component::GetDependencyGraph();

        std::map<std::type_index, std::vector<std::type_index>> G{};
        std::map<std::type_index, int> fanin{};
        std::queue<std::type_index> Q;

        for (auto& entry : m_components)
        {
            for (auto& nextId : dependencyGraph->Query(entry.first))
            {
                auto p = m_components.find(nextId);
                if (p == m_components.end())
                {
                    continue;
                }
                G[entry.first].push_back(p->first);
                fanin[p->first]++;
            }
        }
        for (auto& entry : m_components)
        {
            if (!fanin[entry.first])
            {
                Q.push(entry.first);
            }
        }

        m_components_topological_order.clear();
        while (!Q.empty())
        {
            std::type_index node = Q.front();
            Q.pop();

            m_components_topological_order.push_back(m_components[node]);

            for (auto& v : G[node])
            {
                --fanin[v];
                if (!fanin[v])
                {
                    Q.push(v);
                }
            }
        }
        
        if (m_components_topological_order.size() != m_components.size())
        {
            throw std::runtime_error("Circular dependencies detected!");
        }
    }
}