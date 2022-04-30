#include "MeshComponent.h"
#include <Function/Framework/Components/Transform/TransformComponent.h>

namespace crystal
{

    MeshComponent::MeshComponent(std::shared_ptr<Mesh> mesh)
        : m_mesh(mesh)
    {
        
    }

    MeshComponent::~MeshComponent()
    {}

    void MeshComponent::Update(const GameTimer & gameTimer)
    {}

    void MeshComponent::Draw(const GameTimer & gameTimer)
    {}

    std::vector<std::shared_ptr<IRayHiter>> MeshComponent::CreateRayHiters()
    {
        m_vertices = m_mesh->GetTransformedVertices(m_attachedObject->GetComponent<TransformComponent>()->GetTransform());
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

        std::vector<std::shared_ptr<IRayHiter>> hiters{};
        for (auto& triangle : m_triangles)
        {
            hiters.push_back(std::make_shared<ShapeRayHiter>(m_attachedObject, cptr(triangle)));
        }
        return hiters;
    }
}
