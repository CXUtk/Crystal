#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include "Mesh.h"
#include <Function/Framework/Components/Shape/ShapeRayHiter.h>
#include <Function/Framework/Components/Shape/ShapeAreaSampler.h>
#include <Function/Framework/Components/Shape/Shapes/Triangle.h>

namespace crystal
{
    class MeshComponent : public Component
    {
    public:
        MeshComponent(std::shared_ptr<Mesh> mesh);
        virtual ~MeshComponent() override;

        virtual void Initialize() override;
        virtual void Update(const GameTimer& gameTimer) override;
        virtual void Draw(const GameTimer& gameTimer) override;

        std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
        std::vector<std::shared_ptr<IRayHiter>>     CreateRayHiters() const;
        std::vector<std::shared_ptr<IAreaSampler>>  CreateAreaSampler() const;

    private:
        std::shared_ptr<Mesh>                   m_mesh{};
        std::vector<std::shared_ptr<Triangle>>  m_triangles{};
        std::vector<Float>                      m_weightPrefixSum{};
        std::vector<MeshVertexData>             m_vertices{};
    };
}