#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <typeindex>

#include <Function/Framework/Framework.h>
#include <Core/Utils/GameTimer.h>
#include <Core/Utils/Misc.h>

#include <SJson/SJson.hpp>


namespace crystal
{
    class GameObject
    {
    public:
        static std::shared_ptr<GameObject> LoadGameObject(const SJson::JsonNode& node);

    public:
        GameObject();
        ~GameObject();

        void Initialize();
        void Update(const GameTimer& gameTimer);
        void Draw(const GameTimer& gameTimer);

        std::string GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }

        void AddComponent(std::shared_ptr<Component> component);

        template<typename TComp>
        std::shared_ptr<TComp> GetComponent();

        template<typename TComp>
        std::shared_ptr<const TComp> GetComponent() const;

        template<typename TComp>
        bool HasComponent() const;

    private:
        std::string     m_name{};
        std::map<std::type_index, std::shared_ptr<Component>> m_components{};
        std::vector<std::shared_ptr<Component>> m_components_topological_order{};

        void topologicalSort();
    };

    template<typename TComp>
    inline std::shared_ptr<TComp> GameObject::GetComponent()
    {
        //auto& id = typeid(TComp);
        auto p = m_components.find(typeid(TComp));
        if (p == m_components.end())
        {
            throw std::runtime_error(string_format("Cannot find given component: %s", typeid(TComp).name()));
        }
        return std::dynamic_pointer_cast<TComp>(m_components[typeid(TComp)]);
    }

    template<typename TComp>
    inline std::shared_ptr<const TComp> GameObject::GetComponent() const
    {
        auto p = m_components.find(typeid(TComp));
        if (p == m_components.end())
        {
            throw std::runtime_error(string_format("Cannot find given component: %s", typeid(TComp).name()));
        }
        return std::dynamic_pointer_cast<const TComp>(p->second);
    }

    template<typename TComp>
    inline bool GameObject::HasComponent() const
    {
        return m_components.find(typeid(TComp)) != m_components.end();
    }

}