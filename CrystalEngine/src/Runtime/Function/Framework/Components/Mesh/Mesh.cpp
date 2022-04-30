#include "Mesh.h"

namespace crystal
{
    Mesh::Mesh(const std::vector<MeshVertexData>& V, const std::vector<glm::ivec3>& I)
        : m_vertices(V)
    {}

    Mesh::~Mesh()
    {}

    std::vector<MeshVertexData> Mesh::GetTransformedVertices(const Transform& transform) const
    {
        std::vector<MeshVertexData> newV;
        newV.reserve(m_vertices.size());

        auto matrix = transform.getMatrix();
        auto normalMatrix = glm::transpose(glm::inverse(matrix));
        for (auto& v : m_vertices)
        {
            MeshVertexData newVertex = v;
            newVertex.Position = Point3f(matrix * Vector4f(v.Position, 1.f));
            newVertex.Normal = Point3f(normalMatrix * Vector4f(v.Normal, 0.f));
            newV.push_back(newVertex);
        }
        return newV;
    }
}
