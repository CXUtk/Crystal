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
        RayScene(std::shared_ptr<Scene> scene, std::shared_ptr<CPUTextureCubemap> skybox);

        bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const;
        bool IntersectTest(const Ray3f& ray, float tMin, float tMax) const;
        void ForEachLights(std::function<void(const crystal::Light*)> action) const;

        Spectrum GetEnvironmentLight(const Vector3f& dir) const;

    private:
        std::shared_ptr<Scene>              m_scene = nullptr;
        std::shared_ptr<IAccStructure>      m_accelStructure = nullptr;
        std::shared_ptr<CPUTextureCubemap>  m_skyBox = nullptr;

        std::vector<std::shared_ptr<crystal::IRayPrimitive>>    m_primitives{};
        std::vector<std::shared_ptr<crystal::Light>>            m_lights{};

    };
}