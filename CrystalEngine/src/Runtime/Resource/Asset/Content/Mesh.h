#pragma once
#include <Core/Utils/Math.h>
#include <vector>

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

        const std::vector<MeshVertexData>& GetData() const { return m_vertices; }

    private:
        std::vector<MeshVertexData> m_vertices;
    };
}