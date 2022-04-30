#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <memory>

#include <glm/glm.hpp>

#include <Platform/RHI/FileSystem/FSCommon.h>
#include <Function/Framework/Components/Mesh/Mesh.h>
#include <Core/Math/Math.h>

namespace crystal
{
    //struct TriangleFaceIndex
    //{
    //    int VertexID[3]{},
    //        TexID[3]{},
    //        NormalID[3]{};

    //    TriangleFaceIndex() {}
    //    TriangleFaceIndex(const std::vector<glm::ivec3>& vs)
    //    {
    //        int i = 0;
    //        for (auto& v : vs)
    //        {
    //            VertexID[i] = v[0];
    //            TexID[i] = v[1];
    //            NormalID[i] = v[2];
    //            i++;
    //        }
    //    }
    //    TriangleFaceIndex(int a, int b, int c)
    //    {
    //        VertexID[0] = a;
    //        VertexID[1] = b;
    //        VertexID[2] = c;
    //    }
    //};

    class ObjLoader
    {
    public:
        void load(const path_type& path);
        std::vector<Vector3f> Positions;
        std::vector<Vector2f> TexCoords;
        std::vector<Vector3f> Normals;

        std::vector<glm::ivec3> Triangles;
        std::vector<MeshVertexData> Vertices;

        std::shared_ptr<Mesh> GetMesh();
    private:
        static constexpr int MAX_BUFFER = 100005;
        static char lineBuffer[MAX_BUFFER];
        int _ptr;
        int _totV;


        void process();
        char getNext() { return lineBuffer[++_ptr]; }
        char getCur() { return lineBuffer[_ptr]; }
        char getPrev() { return lineBuffer[--_ptr]; }
        void delSpace()
        {
            char c = lineBuffer[_ptr];
            while (c == ' ' || c == '\n' || c == '\t' || c == '\r')
            {
                c = getNext();
            }
        }
    };
}