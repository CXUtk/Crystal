#pragma once
#include <Core/Math/Math.h>
#include <vector>
#include <Core/Math/Transform.h>
#include <SJson/SJson.hpp>

namespace crystal
{
    struct MeshFaceData
    {
        int V[3]{};
        int Material;
    };
    struct MeshVertexData
    {
        Vector3f Position{};
        Vector2f TexCoord{};
        Vector3f Normal{};
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<MeshVertexData>& V, const std::vector<MeshFaceData>& F,
            const std::vector<std::string> materials);
        Mesh(const std::vector<MeshVertexData>& V, const std::vector<MeshFaceData>& F,
            const std::vector<std::string> materials, const std::map<std::string, SJson::JsonNode>& inits);
        ~Mesh();

        const std::vector<MeshVertexData>& GetVertices() const;
        const std::vector<MeshFaceData>& GetFaces() const;
        const std::vector<std::string>& GetMaterialNames() const { return m_materials; }
        const std::map<std::string, SJson::JsonNode>& GetMaterialInits() const { return m_materialInits; }
        std::vector<MeshVertexData> GetTransformedVertices(const Transform& transform) const;

        bool HasIndices() const { return false; }

    private:
        std::vector<MeshVertexData>     m_vertices{};
        std::vector<MeshFaceData>       m_faces{};
        std::vector<std::string>        m_materials{};

        std::map<std::string, SJson::JsonNode> m_materialInits{};
    };
}