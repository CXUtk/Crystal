#include "RayScene.h"
#include <Function/Framework/Components/Shape/ShapeComponent.h>
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Light/Lights/CubemapEnvironmentLight.h>
#include <Function/Framework/Components/Mesh/MeshComponent.h>

namespace tracer
{
    RayScene::RayScene(std::shared_ptr<Scene> scene, std::shared_ptr<Light> environmentLight)
        : m_scene(scene), m_environmentLight(environmentLight)
    {
        m_accelStructure = IAccStructure::CreateAccelerator("BVH");

        for (auto& obj : scene->GetGameObjects())
        {
            if (obj->HasComponent<ShapeComponent>())
            {
                auto shapeComp = obj->GetComponent<ShapeComponent>();
                m_primitives.push_back(shapeComp->GetRayPrimitive());
            }
            else if (obj->HasComponent<MeshComponent>())
            {
                auto meshComp = obj->GetComponent<MeshComponent>();
                for (auto& triangle : meshComp->GetRayPrimitives())
                {
                    m_primitives.push_back(triangle);
                }
            }

            if (obj->HasComponent<LightComponent>())
            {
                auto& lightComp = obj->GetComponent<LightComponent>();
                auto& lights = lightComp->GetLights();
                m_lights.insert(m_lights.end(), lights.begin(), lights.end());
            }
        }

        if (m_environmentLight != nullptr)
        {
            m_lights.push_back(m_environmentLight);
        }

        for (auto& primitive : m_primitives)
        {
            m_worldBound = m_worldBound.Union(primitive->GetBoundingBox());
        }

        std::vector<Float> weights;
        for (auto& light : m_lights)
        {
            light->Preprocess(m_worldBound);
            auto flux = light->Flux();
            weights.push_back((flux.r + flux.g + flux.b) / 3.f);
        }
        m_lightFluxDistribution = std::make_shared<Distribution1DDiscrete>(weights.data(), weights.size());

        m_accelStructure->Build(m_primitives);
    }

    bool RayScene::Intersect(const Ray3f& ray, SurfaceInteraction* info) const
    {
        return m_accelStructure->Intersect(ray, info);
    }

    bool RayScene::IntersectTest(const Ray3f& ray, float tMin, float tMax) const
    {
        return m_accelStructure->IntersectTest(ray, nullptr, tMin, tMax);
    }

    void RayScene::ForEachLights(std::function<void(const crystal::Light*)> action) const
    {
        for (auto& light : m_lights)
        {
            action(cptr(light));
        }
    }
    crystal::Light* RayScene::SampleOneLight(Float sample, Float* pdf, Float* remapped) const
    {
        size_t index = m_lightFluxDistribution->SampleDiscrete(sample, pdf, remapped);
        return m_lights[index].get();
    }


    Spectrum RayScene::GetEnvironmentLight(const Vector3f& dir) const
    {
        if (!m_environmentLight) return Spectrum(0.f);
        return m_environmentLight->Le(dir);
    }
}
