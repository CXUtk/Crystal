#pragma once
#include <Tracer/Config/RenderProperties.h>
#include <Tracer/AccStructure/IAccStructure.h>

#include <Function/Render/RenderExports.h>
#include <Function/Framework/Scene/Scene.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Medium/Medium.h>

#include <Core/Sampling/Sampler/Sampler.h>
#include <Core/Interaction/RayTr.h>

namespace tracer
{
    class RayScene
    {
    public:
        RayScene(std::shared_ptr<Scene> scene, std::shared_ptr<Light> environmentLight);

        bool IntersectTr(const crystal::RayTr& ray, Sampler* sampler,
            SurfaceInteraction* info, Spectrum* Tr) const;

        bool Intersect(const Ray3f& ray, SurfaceInteraction* info,
            float tMin = 0, float tMax = std::numeric_limits<Float>::infinity()) const;

        bool IntersectTest(const Ray3f& ray, float tMin, float tMax) const;

        void ForEachLights(std::function<void(const crystal::Light*)> action) const;

        const std::vector<std::shared_ptr<crystal::Light>>& GetLights() const { return m_lights; }

        crystal::Light* SampleOneLight(Float sample, Float* pdf, Float* remapped) const;

        Spectrum GetEnvironmentLight(const Vector3f& dir) const;

        Bound3f GetWorldBound() const { return m_worldBound; }

        const std::vector<std::shared_ptr<crystal::IRayPrimitive>>& GetPrimitives() const { return m_primitives; }

        GPUDataPackage GetGPUPackage() const { return m_accelStructure->GetGPUData(); }

    private:
        std::shared_ptr<Scene>              m_scene = nullptr;
        std::shared_ptr<IAccStructure>      m_accelStructure = nullptr;
        std::shared_ptr<Light>              m_environmentLight = nullptr;
        Bound3f                             m_worldBound{};
        std::shared_ptr<Distribution1DDiscrete>   m_lightFluxDistribution = nullptr;

        std::vector<std::shared_ptr<crystal::IRayPrimitive>>    m_primitives{};
        std::vector<std::shared_ptr<crystal::Light>>            m_lights{};

    };
}