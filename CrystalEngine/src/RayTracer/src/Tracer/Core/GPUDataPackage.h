#pragma once
#include <Tracer/Config/RenderProperties.h>

#include <Function/Render/RenderExports.h>
#include <Function/Framework/Scene/Scene.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Medium/Medium.h>
#include <Function/Framework/Components/Shape/Shapes/Triangle.h>

#include <Core/Sampling/Sampler/Sampler.h>
#include <Core/Interaction/RayTr.h>

namespace tracer
{
    class GPUDataPackage
    {
    public:
        GPUDataPackage();
        ~GPUDataPackage();

        size_t AddBVHNode(const Bound3f& bound);
        void SetBVHNodeLeft(size_t target, size_t left);
        void SetBVHNodeRight(size_t target, size_t right);
        size_t AddObjectsNode(const Bound3f& bound, const std::vector<const crystal::IRayPrimitive*>& primitives);

        void AddObject(const crystal::IRayPrimitive* primitive);

        const std::vector<Vector4f>& GetObjectData() const { return m_GPUObjectData; }
        const std::vector<Float>& GetSceneData() const { return m_GPUSceneData; }

    private:
        std::vector<Vector4f>                           m_GPUObjectData{};
        std::vector<Float>                              m_GPUSceneData{};
        std::map<const crystal::IRayPrimitive*, int>    m_offsetTable{};

        size_t      m_ObjectDataPtr{};
        size_t      m_SceneDataPtr{};

        size_t WriteObjectData(const Vector4f& v);
        size_t WriteSceneData(Float v);


        void ModifySceneData(size_t index, Float v);
        void ModifyObjectData(size_t index, const Vector4f& v);

        void AddTriangle(const crystal::IRayPrimitive* triangle,
            const Vector3f& abeldo);
    };
}