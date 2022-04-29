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


namespace crystal
{
    class GameObject
    {
    public:
        GameObject();
        virtual ~GameObject();

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

        std::string GetName() const { return m_name; }

        void AddComponent(std::shared_ptr<Component> component);

        template<typename TComp>
        std::shared_ptr<TComp> GetComponent();

        template<typename TComp>
        std::shared_ptr<const TComp> GetComponent() const;

        template<typename TComp>
        bool HasComponent() const;

    private:
        std::string     m_name;

        std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
    };

    template<typename TComp>
    inline std::shared_ptr<TComp> GameObject::GetComponent()
    {
        //auto& id = typeid(TComp);
        auto p = m_components.find(typeid(TComp));
        if (p == m_components.end())
        {
            throw std::runtime_error(string_format("Cannot find given component: %s", id.name()));
        }
        return std::dynamic_pointer_cast<TComp>(m_components[typeid(TComp)]);
    }

    template<typename TComp>
    inline std::shared_ptr<const TComp> GameObject::GetComponent() const
    {
        return std::dynamic_pointer_cast<const TComp>(m_components[typeid(TComp)]);
    }

    template<typename TComp>
    inline bool GameObject::HasComponent() const
    {
        return m_components.find(id) != m_components.end();
    }

}