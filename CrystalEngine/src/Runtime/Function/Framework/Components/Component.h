#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>

#include <Function/Framework/Framework.h>
#include <Core/Utils/GameTimer.h>
#include <typeindex>

#include <SJson/SJson.hpp>

namespace crystal
{

    class ComponentDependencyGraph
    {
    public:
        ComponentDependencyGraph() {}
        ~ComponentDependencyGraph() {}

        // There is an U->V edge means that U must be initialize before V
        template<typename U, typename V>
        void AddEdge();

        const std::vector<std::type_index> Query(const std::type_index& typeindex) const;

        std::map<std::type_index, std::vector<std::type_index>> m_adjacencyList{};
    };

    template<typename U, typename V>
    inline void ComponentDependencyGraph::AddEdge()
    {
        m_adjacencyList[typeid(U)].push_back(typeid(V));
    }

    class Component
    {
    public:
        static const ComponentDependencyGraph* GetDependencyGraph();

        Component();
        virtual ~Component() = 0;

        // Initialize with possible dependencies on other components
        virtual void Initialize() = 0;
        virtual void Update(const GameTimer& gameTimer) = 0;
        virtual void Draw(const GameTimer& gameTimer) = 0;

        void AttachToObject(GameObject* gameObject) { m_attachedObject = gameObject; }
        GameObject* GetAttachedObject() const { return m_attachedObject; }

    protected:
        GameObject* m_attachedObject = nullptr;

    private:
    };


}