#include "GPUDataPackage.h"
#include <Function/Framework/Components/Light/Lights/AreaLight.h>
#include <Function/Framework/Components/Light/Lights/DiffusedAreaLight.h>

using namespace crystal;
namespace tracer
{
    //struct BVHNode
    //{
    //  Float type;
    //  Float ptrBBox;
    //  Float splitAxis;
    //  Float left;
    //  Float right;
    //};
    //struct BVHNode_Obj
    //{
    //  Float type;
    //  Float ptrBBox;
    //  Float nPrimitives;
    //  Float objs[nPrimitives];
    //};

    static constexpr size_t BVHNode_Size = 4;

    static constexpr float RT_GEOMETRY_COLLECTION = 1.0;
    static constexpr float RT_GEOMETRY_BVH_NODE = 2.0;

    static constexpr float RT_GEOMETRY_SPHERE = 1.0;
    static constexpr float RT_GEOMETRY_TRIANGLE = 2.0;


    static constexpr float RT_LIGHT_POINT = 1.0;
    static constexpr float RT_LIGHT_AREA_DIFFUSE = 2.0;
    static constexpr float RT_LIGHT_INFINITE = 3.0;

    GPUDataPackage::GPUDataPackage()
    {}

    GPUDataPackage::~GPUDataPackage()
    {}

    size_t GPUDataPackage::AddBVHNode(const Bound3f & bound, int splitAxis)
    {
        size_t dataStart = WriteObjectData(Vector4f(bound.GetMinPos(), 1));
        WriteObjectData(Vector4f(bound.GetMaxPos(), 1));

        // type
        size_t nodeStart = WriteSceneData(RT_GEOMETRY_BVH_NODE);

        // ptrBBox
        WriteSceneData(dataStart);

        // splitAxis
        WriteSceneData(splitAxis);

        // Left
        WriteSceneData(-1.f);

        // Right
        WriteSceneData(-1.f);
        return nodeStart;
    }

    void GPUDataPackage::SetBVHNodeLeft(size_t target, size_t left)
    {
        ModifySceneData(target + 3, left);
    }

    void GPUDataPackage::SetBVHNodeRight(size_t target, size_t right)
    {
        ModifySceneData(target + 4, right);
    }

    size_t GPUDataPackage::AddObjectsNode(const Bound3f& bound, const std::vector<const crystal::IRayPrimitive*>& primitives)
    {
        size_t dataStart = WriteObjectData(Vector4f(bound.GetMinPos(), 1));
        WriteObjectData(Vector4f(bound.GetMaxPos(), 1));

        // type
        size_t nodeStart = WriteSceneData(RT_GEOMETRY_COLLECTION);

        // ptrBBox
        WriteSceneData(dataStart);

        // nPrimitives
        WriteSceneData(primitives.size());

        // primitives
        for (auto& p : primitives)
        {
            WriteSceneData(m_offsetTablePrimitive[p]);
        }
        return nodeStart;
    }

    void GPUDataPackage::AddLights(const std::vector<std::shared_ptr<crystal::Light>>& lights)
    {
        WriteSceneData(lights.size());
        for (auto& p : lights)
        {
            WriteSceneData(m_offsetTableLight[p.get()]);
        }
    }

    void GPUDataPackage::AddObject(const crystal::IRayPrimitive * primitive)
    {
        if (dynamic_cast<const Triangle*>(primitive->GetShape()) != nullptr)
        {
            AddTriangle(primitive, Vector3f(1.f, 1.f, 1.f));
        }

        if (primitive->GetAreaLight() != nullptr)
        {
            AddAreaLight(primitive->GetAreaLight(), primitive);
        }
    }

    void GPUDataPackage::AddLight(const crystal::Light* light)
    {
        if (m_offsetTableLight.find(light) == m_offsetTableLight.end())
        {
            // Add light
            size_t dataStart = WriteObjectData(Vector4f(RT_LIGHT_INFINITE));
            m_offsetTableLight[light] = dataStart;
        }
        
    }

    size_t GPUDataPackage::WriteObjectData(const Vector4f& v)
    {
        m_GPUObjectData.push_back(v);
        return m_ObjectDataPtr++;
    }

    size_t GPUDataPackage::WriteSceneData(Float v)
    {
        m_GPUSceneData.push_back(v);
        return m_SceneDataPtr++;
    }

    void GPUDataPackage::ModifySceneData(size_t index, Float v)
    {
        m_GPUSceneData[index] = v;
    }

    void GPUDataPackage::ModifyObjectData(size_t index, const Vector4f& v)
    {
        m_GPUObjectData[index] = v;
    }

    void GPUDataPackage::AddTriangle(const IRayPrimitive* triangle, const Vector3f& abeldo)
    {
        auto tri = (const Triangle*)triangle->GetShape();
        auto& v = tri->GetVertices();
        size_t dataStart = WriteObjectData(Vector4f(RT_GEOMETRY_TRIANGLE));
        WriteObjectData(Vector4f(v[0]->Position, 1));
        WriteObjectData(Vector4f(v[1]->Position, 1));
        WriteObjectData(Vector4f(v[2]->Position, 1));
        WriteObjectData(Vector4f(v[0]->Normal, 1));
        WriteObjectData(Vector4f(v[1]->Normal, 1));
        WriteObjectData(Vector4f(v[2]->Normal, 1));
        WriteObjectData(Vector4f(v[0]->TexCoord, 0, 1));
        WriteObjectData(Vector4f(v[1]->TexCoord, 0, 1));
        WriteObjectData(Vector4f(v[2]->TexCoord, 0, 1));
        WriteObjectData(Vector4f(abeldo, 1));
        m_offsetTablePrimitive[triangle] = dataStart;
    }

    void GPUDataPackage::AddAreaLight(const crystal::AreaLight* areaLight, const crystal::IRayPrimitive* primitive)
    {
        //struct Light
        //{
        //    float type;
        //    params ...
        //};

        //struct DiffuseAreaLight
        //{
        //  float primitveOffset;
        // float3 Le;
        //};
        if (dynamic_cast<const crystal::DiffusedAreaLight*>(areaLight) != nullptr)
        {
            auto light = dynamic_cast<const crystal::DiffusedAreaLight*>(areaLight);
            size_t dataStart = WriteObjectData(Vector4f(RT_LIGHT_AREA_DIFFUSE));
            assert(m_offsetTablePrimitive.find(primitive) != m_offsetTablePrimitive.end());
            size_t s = m_offsetTablePrimitive[primitive];
            WriteObjectData(Vector4f(s, light->GetLe()));
            m_offsetTableLight[areaLight] = dataStart;
        }

    }
}