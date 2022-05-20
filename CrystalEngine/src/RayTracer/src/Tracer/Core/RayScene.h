#pragma once
#include <Tracer/Config/RenderProperties.h>
#include <Tracer/AccStructure/IAccStructure.h>

#include <Function/Render/RenderExports.h>
#include <Function/Framework/Scene/Scene.h>
#include <Function/Framework/Components/Light/LightComponent.h>

namespace tracer
{
    class RayScene
    {
    public:
        RayScene(std::shared_ptr<Scene> scene, std::shared_ptr<Light> environmentLight);

        bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const;
        bool IntersectTest(const Ray3f& ray, float tMin, float tMax) const;
        void ForEachLights(std::function<void(const crystal::Light*)> action) const;
        const std::vector<std::shared_ptr<crystal::Light>>& GetLights() const { return m_lights; }
        crystal::Light* SampleOneLight(Float sample, Float* pdf, Float* remapped) const;

        Spectrum GetEnvironmentLight(const Vector3f& dir) const;

        Bound3f GetWorldBound() const { return m_worldBound; }

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