#include "Mesh.h"

namespace crystal
{
    Mesh::Mesh(const std::vector<VertexData>& V, const std::vector<glm::ivec3>& I)
        : m_vertices(V)
    {}

    Mesh::~Mesh()
    {}
}
