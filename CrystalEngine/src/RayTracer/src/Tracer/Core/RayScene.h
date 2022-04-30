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
        RayScene(std::shared_ptr<Scene> scene);

        bool Intersect(const Ray3f& ray, SurfaceInteraction* info) const;
        bool IntersectTest(const Ray3f& ray, float tMin, float tMax) const;
        void ForEachLights(std::function<void(const crystal::LightComponent*)> action) const;

    private:
        std::shared_ptr<Scene>          m_scene = nullptr;
        std::shared_ptr<IAccStructure>  m_accelStructure = nullptr;

        std::vector<std::shared_ptr<crystal::IRayHiter>>         m_hiters{};
        std::vector<std::shared_ptr<crystal::LightComponent>>    m_light{};
    };
}