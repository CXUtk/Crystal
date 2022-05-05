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

        auto& Faces = m_mesh->GetFaces();

        for (auto& face : Faces)
        {
            m_triangles[face.Material].push_back(std::make_shared<Triangle>(
                &m_vertices[face.V[0]],
                &m_vertices[face.V[1]],
                &m_vertices[face.V[2]]));
        }

        for (auto& pair : m_mesh->GetMaterialInits())
        {
            m_materials[pair.first] = Material::CreateMaterial(pair.second);
        }

        //if (!m_mesh->HasIndices())
        //{
        //    size_t size = V.size();
        //    for (int i = 0; i < size; i += 3)
        //    {
        //        m_triangles.push_back(std::make_shared<Triangle>(&V[i], &V[i + 1], &V[i + 2]));
        //    }
        //}
        //else
        //{
        //    //auto& I = m_mesh->GetIndices();
        //    //size_t size = I.size();
        //    //for (int i = 0; i < size; i++)
        //    //{
        //    //    auto& triangle = I[i];
        //    //    m_triangles.push_back(std::make_shared<Triangle>(&V[triangle.x], &V[triangle.y], &V[triangle.z]));
        //    //}
        //}
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

        bool useModelMaterial = true;
        // If there is a material component, then use the information in the component
        if (m_attachedObject->HasComponent<MaterialComponent>())
        {
            auto&& materialComp = m_attachedObject->GetComponent<MaterialComponent>();
            material = materialComp->GetMaterial();
            useModelMaterial = false;
        }

        int index = 0;
        const auto& matNames = m_mesh->GetMaterialNames();
        // Otherwise, use the material information from model file
        for (auto& pair : m_triangles)
        {
            if (pair.first != -1 && useModelMaterial)
            {
                // Find material according to this matID
                auto& name = matNames[pair.first];
                material = FindMaterial(name);
            }

            for (auto& triangle : pair.second)
            {
                const AreaLight* light = nullptr;
                if (index < areaLights.size())
                {
                    light = areaLights[index];
                }
                primitives.push_back(std::make_shared<ShapeRayPrimitive>(cptr(triangle), light, material));

                ++index;
            }
        }
        return primitives;
    }

    void MeshComponent::ForeachTriangle(std::function<void(std::shared_ptr<const Triangle>)> action) const
    {
        for (auto& pair : m_triangles)
        {
            for (auto& triangle : pair.second)
            {
                action(triangle);
            }
        }
    }
    const Material* MeshComponent::FindMaterial(const std::string& name) const
    {
        auto p = m_materials.find(name);
        if (p == m_materials.end())
        {
            throw std::runtime_error("Cannot find such material");
        }
        return cptr(p->second);
    }
}
