#pragma once
#include <Core/Math/Math.h>
#include <vector>
#include <Core/Math/Transform.h>

namespace crystal
{
    struct MeshVertexData
    {
        Vector3f Position{};
        Vector2f TexCoord{};
        Vector3f Normal{};
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<MeshVertexData>& V, const std::vector<glm::ivec3>& I);
        ~Mesh();

        const std::vector<MeshVertexData>& GetVertices() const { return m_vertices; }
        const std::vector<glm::ivec3>& GetIndices() const { return m_indices; }
        std::vector<MeshVertexData> GetTransformedVertices(const Transform& transform) const;

        bool HasIndices() const { return !m_indices.empty(); }

    private:
        std::vector<MeshVertexData> m_vertices{};
        std::vector<glm::ivec3>     m_indices{};
    };
}