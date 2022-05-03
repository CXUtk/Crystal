#include "MeshComponent.h"
#include <Function/Framework/Components/Transform/TransformComponent.h>
#include <Function/Framework/Components/Light/ComponentSettings.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>

#include <Engine.h>
#include <Resource/Asset/AssetManager.h>

namespace crystal
{

    MeshComponent::MeshComponent(std::shared_ptr<Mesh> mesh)
        : m_mesh(mesh)
    {
        
    }

    MeshComponent::MeshComponent(const SJson::JsonNode& node)
        : m_setting(node)
    {}

    MeshComponent::~MeshComponent()
    {}

    void MeshComponent::Initialize()
    {
        if (m_setting.GetType() != SJson::ValueType::Null)
        {
            auto assetManager = Engine::GetInstance()->GetAssetManager();
            auto modelURI = m_setting["Model"].Get<std::string>();
            m_mesh = assetManager->LoadAsset<Mesh>(modelURI);
        }

        auto& transform = m_attachedObject->GetComponent<TransformComponent>()->GetTransform();
        m_vertices = m_mesh->GetTransformedVertices(transform);

        auto& V = m_vertices;
        if (!m_mesh->HasIndices())
        {
            size_t size = V.size();
            for (int i = 0; i < size; i += 3)
            {
                m_triangles.push_back(std::make_shared<Triangle>(&V[i], &V[i + 1], &V[i + 2]));
            }
        }
        else
        {
            auto& I = m_mesh->GetIndices();
            size_t size = I.size();
            for (int i = 0; i < size; i++)
            {
                auto& triangle = I[i];
                m_triangles.push_back(std::make_shared<Triangle>(&V[triangle.x], &V[triangle.y], &V[triangle.z]));
            }
        }
    }

    void MeshComponent::Update(const GameTimer & gameTimer)
    {}

    void MeshComponent::Draw(const GameTimer & gameTimer)
    {}

    std::vector<std::shared_ptr<IRayPrimitive>> MeshComponent::GetRayPrimitives() const
    {
        std::vector<std::shared_ptr<IRayPrimitive>> primitives{};
        std::vector<const AreaLight*> areaLights;
        const Material* material = nullptr;

        if (m_attachedObject->HasComponent<LightComponent>())
        {
            auto&& lightComp = m_attachedObject->GetComponent<LightComponent>();

            for (auto& light : lightComp->GetLights())
            {
                if (light->IsAreaLight())
                {
                    areaLights.push_back(dynamic_cast<const AreaLight*>(light.get()));
                }
                else
                {
                    areaLights.push_back(nullptr);
                }
            }
        }
        if (m_attachedObject->HasComponent<MaterialComponent>())
        {
            auto&& materialComp = m_attachedObject->GetComponent<MaterialComponent>();
            material = materialComp->GetMaterial();
        }
        size_t count = m_triangles.size();
        size_t lightCount = areaLights.size();
        for (size_t i = 0; i < count; i++)
        {
            auto& triangle = m_triangles[i];
            const AreaLight* light = nullptr;
            if (i < lightCount)
            {
                light = areaLights[i];
            }

            primitives.push_back(std::make_shared<ShapeRayPrimitive>(cptr(triangle), light, material));
        }
        return primitives;
    }

    void MeshComponent::ForeachTriangle(std::function<void(std::shared_ptr<const Triangle>)> action) const
    {
        for (auto& triangle : m_triangles)
        {
            action(triangle);
        }
    }
}
