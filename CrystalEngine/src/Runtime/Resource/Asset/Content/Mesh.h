#pragma once
#include <Core/Utils/Math.h>
#include <vector>

namespace crystal
{
    struct VertexData
    {
        Vector3f Position{};
        Vector2f TexCoord{};
        Vector3f Normal{};
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<VertexData>& V, const std::vector<glm::ivec3>& I);
        ~Mesh();

    private:
        std::vector<VertexData> m_vertices;
    };
}