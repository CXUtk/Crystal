#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include "Mesh.h"
#include <Function/Framework/Components/Shape/ShapeRayPrimitive.h>
#include <Function/Framework/Components/Shape/ShapeAreaSampler.h>
#include <Function/Framework/Components/Shape/Shapes/Triangle.h>

namespace crystal
{
    class MeshComponent : public Component
    {
    public:
        MeshComponent(std::shared_ptr<Mesh> mesh);

        // ["Model"] = URI to model mesh
        MeshComponent(const SJson::JsonNode& node);
        virtual ~MeshComponent() override;

        virtual void Initialize() override;
        virtual void Update(const GameTimer& gameTimer) override;
        virtual void Draw(const GameTimer& gameTimer) override;

        std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
        std::vector<std::shared_ptr<IRayPrimitive>> GetRayPrimitives() const;

        void ForeachTriangle(std::function<void(std::shared_ptr<const Triangle>)> action) const;

    private:
        SJson::JsonNode                         m_setting{};
        std::shared_ptr<Mesh>                   m_mesh{};
        std::vector<MeshVertexData>             m_vertices{};

        std::map<std::string, std::shared_ptr<Material>>        m_materials{};
        std::map<int, std::vector<std::shared_ptr<Triangle>>>   m_triangles{};

        const Material* FindMaterial(const std::string& name) const;
    };
}