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

        size_t AddBVHNode(const Bound3f& bound, int splitAxis);
        void SetBVHNodeLeft(size_t target, size_t left);
        void SetBVHNodeRight(size_t target, size_t right);
        size_t AddObjectsNode(const Bound3f& bound, const std::vector<const crystal::IRayPrimitive*>& primitives);

        void AddLights(const std::vector<std::shared_ptr<crystal::Light>>& lights);

        void AddObject(const crystal::IRayPrimitive* primitive);
        void AddLight(const crystal::Light* light);

        const std::vector<Vector4f>& GetObjectData() const { return m_GPUObjectData; }
        const std::vector<Float>& GetSceneData() const { return m_GPUSceneData; }


        size_t WriteObjectData(const Vector4f& v);
        size_t WriteSceneData(Float v);

        void ModifySceneData(size_t index, Float v);
        void ModifyObjectData(size_t index, const Vector4f& v);

        size_t GetObjectDataPtr() const { return m_ObjectDataPtr; }
        size_t GetSceneDataPtr() const { return m_SceneDataPtr; }

    private:
        std::vector<Vector4f>                           m_GPUObjectData{};
        std::vector<Float>                              m_GPUSceneData{};
        std::map<const crystal::IRayPrimitive*, int>    m_offsetTablePrimitive{};
        std::map<const crystal::Light*, int>            m_offsetTableLight{};

        size_t      m_ObjectDataPtr{};
        size_t      m_SceneDataPtr{};


        void AddTriangle(const crystal::IRayPrimitive* triangle,
            const Vector3f& abeldo);
        void AddAreaLight(const crystal::AreaLight* areaLight,
            const crystal::IRayPrimitive* primitive);
    };
}