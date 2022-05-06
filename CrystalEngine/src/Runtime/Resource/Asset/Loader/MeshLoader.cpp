#include "MeshLoader.h"

#include <Core/Utils/Logger.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

namespace crystal
{
    std::shared_ptr<Mesh> MeshLoader::LoadOneMeshFromObj(const SJson::JsonNode& metaInfo,
        const path_type& path, int index)
    {
        auto target = metaInfo["target"].Get<std::string>();
        auto pathToTarget = path / target;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, pathToTarget.string().c_str(),
            path.string().c_str());
        if (!err.empty())
        {
            GlobalLogger::Log(SeverityLevel::Warning, "OBJLoader Warning: %s", err.c_str());
        }

        if (!ret)
        {
            return nullptr;
        }


        std::vector<MeshFaceData> Triangles{};
        std::vector<MeshVertexData> Vertices{};
        std::vector<std::string> Materials{};


        size_t v_offset = 0;
        for (auto& shape : shapes)
        {
            size_t index_offset = 0;
            size_t faces = shape.mesh.num_face_vertices.size();
            for (size_t f = 0; f < faces; f++)
            {
                size_t fnum = shape.mesh.num_face_vertices[f];
                assert(fnum == 3);

                MeshFaceData faceData = {};

                // For each vertex in the face
                for (size_t v = 0; v < fnum; v++)
                {
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                    faceData.V[v] = v_offset + index_offset + v;

                    MeshVertexData vertexData = {};
                    vertexData.Position = Vector3f(attrib.vertices[idx.vertex_index * 3],
                        attrib.vertices[idx.vertex_index * 3 + 1],
                        attrib.vertices[idx.vertex_index * 3 + 2]);

                    vertexData.Normal = Vector3f(attrib.normals[idx.normal_index * 3],
                        attrib.normals[idx.normal_index * 3 + 1],
                        attrib.normals[idx.normal_index * 3 + 2]);

                    vertexData.TexCoord = Vector2f(attrib.texcoords[idx.texcoord_index * 2],
                        attrib.texcoords[idx.texcoord_index * 2 + 1]);

                    Vertices.push_back(vertexData);

                    //printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
                    //       static_cast<long>(v), idx.vertex_index, idx.normal_index,
                    //       idx.texcoord_index);
                }

                faceData.Material = shape.mesh.material_ids[f];
                //printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
                //       shapes[i].mesh.material_ids[f]);
                //printf("  face[%ld].smoothing_group_id = %d\n", static_cast<long>(f),
                //       shapes[i].mesh.smoothing_group_ids[f]);

                index_offset += fnum;
                Triangles.push_back(faceData);
            }

            v_offset += index_offset;
        }

        for (auto& material : materials)
        {
            Materials.push_back(material.name);
        }



        if (metaInfo.Contains("material"))
        {
            std::map<std::string, SJson::JsonNode> materialInits;

            auto pathToMaterial = path / metaInfo["material"].Get<std::string>();
            auto MTLs = SJson::JsonConvert::Parse(File::ReadAllText(pathToMaterial));
            MTLs.foreach_pairs([&](const std::string& name, const SJson::JsonNode& node) {
                materialInits[name] = node;
            });

            return std::make_shared<Mesh>(Vertices, Triangles, Materials, materialInits);
        }


        return std::make_shared<Mesh>(Vertices, Triangles, Materials);
    }
}
