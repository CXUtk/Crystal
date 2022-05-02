#include "Component.h"

#include <Function/Framework/Components/Transform/TransformComponent.h>
#include <Function/Framework/Components/Mesh/MeshComponent.h>
#include <Function/Framework/Components/Shape/ShapeComponent.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Camera/CameraComponent.h>
#include <Function/Framework/Components/Material/MaterialComponent.h>

namespace crystal
{
    const ComponentDependencyGraph* Component::GetDependencyGraph()
    {
        static ComponentDependencyGraph g_dependencyGraph{};
        static bool g_isDependencyGraphInitialized = false;

        if (!g_isDependencyGraphInitialized)
        {
            g_dependencyGraph.AddEdge<TransformComponent, ShapeComponent>();
            g_dependencyGraph.AddEdge<TransformComponent, LightComponent>();
            g_dependencyGraph.AddEdge<TransformComponent, MeshComponent>();
            g_dependencyGraph.AddEdge<TransformComponent, CameraComponent>();

            g_dependencyGraph.AddEdge<ShapeComponent, LightComponent>();
            g_dependencyGraph.AddEdge<MeshComponent, LightComponent>();

            g_isDependencyGraphInitialized = true;
        }
        return &g_dependencyGraph;
    }

    const std::vector<std::type_index> ComponentDependencyGraph::Query(const std::type_index& typeindex) const
    {
        auto p = m_adjacencyList.find(typeindex);
        if (p == m_adjacencyList.end()) return std::vector<std::type_index>();
        return p->second;
    }

    Component::Component()
    {}

    Component::~Component()
    {
        m_attachedObject = nullptr;
    }

    void Component::Initialize()
    {}

    void Component::Update(const GameTimer& gameTimer)
    {}

    void Component::Draw(const GameTimer& gameTimer)
    {}


}
