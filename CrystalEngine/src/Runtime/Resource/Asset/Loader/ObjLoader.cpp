//#include "ObjLoader.h"
//#include <map>
//#include <glm/gtx/transform.hpp>
//#include <Core/Utils/Misc.h>
//
//namespace crystal
//{
//    char ObjLoader::lineBuffer[MAX_BUFFER];
//    struct cmpVec3
//    {
//        bool operator()(const glm::vec3& A, const glm::vec3& B) const
//        {
//            for (int i = 0; i < 3; i++)
//            {
//                if (A[i] != B[i])
//                {
//                    if (A[i] < B[i]) return true;
//                    else return false;
//                }
//            }
//            return false;
//        }
//    };
//    void ObjLoader::load(const path_type& path)
//    {
//        Positions.clear();
//        TexCoords.clear();
//        Normals.clear();
//
//        Vertices.clear();
//        Triangles.clear();
//
//        _totV = 0;
//
//        FILE* file = fopen(path.string().c_str(), "r");
//        if (!file)
//        {
//            throw std::exception(string_format("Cannot open file %s", path.string().c_str()).c_str());
//            return;
//        }
//        while (fgets(lineBuffer, MAX_BUFFER, file))
//        {
//            _ptr = 0;
//            process();
//        }
//        fclose(file);
//    }
//
//
//    int fix(int id, int maxsz)
//    {
//        if (id < 0) return maxsz + id;
//        return id;
//    }
//
//
//    bool readInt(const char* S, int& idx, int& num)
//    {
//        num = 0;
//        while (S[idx] == ' ') idx++;
//        bool neg = false;
//        if (S[idx] == '-')
//        {
//            neg = true;
//            idx++;
//        }
//        else if (S[idx] == '+')
//        {
//            idx++;
//        }
//        while (isdigit(S[idx]))
//        {
//            num *= 10;
//            num += S[idx] - '0';
//            idx++;
//        }
//        if (neg) num *= -1;
//        return true;
//    }
//
//    std::shared_ptr<Mesh> ObjLoader::GetMesh()
//    {
//        return std::make_shared<Mesh>(Vertices, Triangles);
//    }
//
//    //std::vector<DrawTriangle> ObjLoader::GetDrawTriangles() const {
//    //    std::vector<DrawTriangle> triangles;
//    //    for (auto face : Triangles) {
//    //        DrawTriangle tri;
//    //        DrawVertex v1[3];
//    //        for (int i = 0; i < 3; i++) {
//    //            v1[i].pos = Vertices[face.v[i]];
//    //            v1[i].normal = Normals[face.v[i]];
//    //            tri.V[i] = v1[i];
//    //        }
//    //
//    //        triangles.push_back(tri);
//    //    }
//    //    return triangles;
//    //}
//
//
//
//    void ObjLoader::process()
//    {
//        static char start[16];
//        static char faceV[105];
//        sscanf(lineBuffer + _ptr, "%s", start);
//        _ptr += strlen(start);
//
//        if (!strcmp(start, "v"))
//        {
//            double x, y, z;
//            _ptr += sscanf(lineBuffer + _ptr, "%lf%lf%lf", &x, &y, &z);
//            _totV++;
//            auto pt = glm::vec3(x, y, z);
//            Positions.push_back(pt);
//        }
//        else if (!strcmp(start, "f"))
//        {
//            int index = 0;
//            int c = 0;
//            int vd[3]{}, vt[3]{}, vn[3]{};
//            while (~(c = sscanf(lineBuffer + _ptr, "%s", faceV)))
//            {
//                int id = 0;
//                bool b = readInt(faceV, id, vd[index]);
//                id++;
//                readInt(faceV, id, vt[index]);
//                id++;
//                readInt(faceV, id, vn[index]);
//                index++;
//                _ptr += strlen(faceV) + 1;
//            }
//            MeshVertexData V[3];
//            memset(V, 0, sizeof(V));
//            for (int k = 0; k < 3; k++)
//            {
//                V[k].Position = Positions[vd[k] - 1];
//            }
//            if (vn[0] != 0)
//            {
//                for (int k = 0; k < 3; k++)
//                {
//                    V[k].Normal = Normals[vn[k] - 1];
//                }
//            }
//            if (vt[0] != 0)
//            {
//                for (int k = 0; k < 3; k++)
//                {
//                    V[k].TexCoord = TexCoords[vt[k] - 1];
//                }
//            }
//            int cur = Vertices.size();
//            Triangles.push_back(glm::ivec3(cur, cur + 1, cur + 2));
//            for (int k = 0; k < 3; k++)
//            {
//                Vertices.push_back(V[k]);
//            }
//
//        }
//        else if (!strcmp(start, "vt"))
//        {
//            double x, y;
//            _ptr += sscanf(lineBuffer + _ptr, "%lf%lf", &x, &y);
//            TexCoords.push_back(glm::vec2(x, y));
//        }
//        else if (!strcmp(start, "vn"))
//        {
//            double x, y, z;
//            _ptr += sscanf(lineBuffer + _ptr, "%lf%lf%lf", &x, &y, &z);
//            Normals.push_back(glm::vec3(x, y, z));
//        }
//    }
//}