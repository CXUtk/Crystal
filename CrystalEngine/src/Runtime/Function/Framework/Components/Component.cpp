#include "Component.h"

#include <Function/Framework/Components/Transform/TransformComponent.h>
#include <Function/Framework/Components/Mesh/MeshComponent.h>
#include <Function/Framework/Components/Shape/ShapeComponent.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Camera/CameraComponent.h>
#include <Function/Framework/Components/Material/MaterialComponent.h>

namespace crystal
{
    std::shared_ptr<Component> Component::LoadComponent(const SJson::JsonNode& node)
    {
        ComponentType type = SRefl::EnumInfo<crystal::ComponentType>::string_to_enum(
            node["Type"].Get<std::string>()
        );
        auto& setting = node["Setting"];
        switch (type)
        {
        case crystal::ComponentType::CameraComponent:
        {
            return std::make_shared<CameraComponent>(setting);
        }
        case crystal::ComponentType::LightComponent:
        {
            return std::make_shared<LightComponent>(setting);
        }
        case crystal::ComponentType::MaterialComponent:
        {
            return std::make_shared<MaterialComponent>(setting);
        }
        case crystal::ComponentType::MeshComponent:
        {
            return std::make_shared<MeshComponent>(setting);
        }
        case crystal::ComponentType::ShapeComponent:
        {
            return std::make_shared<ShapeComponent>(setting);
        }
        case crystal::ComponentType::TransformComponent:
        {
            auto transform = SJson::de_serialize<Transform>(setting);
            return std::make_shared<TransformComponent>(transform);
        }
        default:
            break;
        }
        return nullptr;
    }

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
