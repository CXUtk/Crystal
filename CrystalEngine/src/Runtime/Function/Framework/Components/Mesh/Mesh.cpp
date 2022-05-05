#include "Mesh.h"
#include <SJson/SJson.hpp>

namespace crystal
{
    Mesh::Mesh(const std::vector<MeshVertexData>& V, const std::vector<MeshFaceData>& F,
        const std::vector<std::string> materials)
        : m_vertices(V), m_faces(F), m_materials(materials)
    {}

    Mesh::Mesh(const std::vector<MeshVertexData>& V, const std::vector<MeshFaceData>& F,
        const std::vector<std::string> materials, const std::map<std::string, SJson::JsonNode>& inits)
        : m_vertices(V), m_faces(F), m_materials(materials), m_materialInits(inits)
    {}

    Mesh::~Mesh()
    {}

    const std::vector<MeshVertexData>& Mesh::GetVertices() const
    {
        return m_vertices;
    }

    const std::vector<MeshFaceData>& Mesh::GetFaces() const
    {
        return m_faces;
    }

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
