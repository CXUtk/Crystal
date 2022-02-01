#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <memory>

#include <glm/glm.hpp>
namespace objloader
{
    struct TriangleFaceIndex
    {
        int VertexID[3], TexID[3], NormalID[3];
        TriangleFaceIndex() {}
        TriangleFaceIndex(const std::vector<std::tuple<int, int, int>>& vs)
        {
            int i = 0;
            for (auto& [a, b, c] : vs)
            {
                VertexID[i] = a;
                TexID[i] = b;
                NormalID[i] = c;
                i++;
            }
        }
        TriangleFaceIndex(int a, int b, int c)
        {
            VertexID[0] = a;
            VertexID[1] = b;
            VertexID[2] = c;
        }
    };

    struct VertexData
    {
        glm::vec3 Position;
        glm::vec2 TexCoord;
        glm::vec3 Normal;
    };

    class ObjLoader
    {
    public:
        void load(const std::string& path);
        std::vector<glm::vec3> Positions;
        std::vector<glm::vec2> TexCoords;
        std::vector<glm::vec3> Normals;

        std::vector<glm::ivec3> Triangles;
        std::vector<VertexData> Vertices;

        //std::vector<DrawTriangle> GetDrawTriangles() const;
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