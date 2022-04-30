#include "LightComponent.h"
#include <Function/Framework/Components/Transform/TransformComponent.h>

namespace crystal
{
    LightComponent::LightComponent(std::shared_ptr<Light> light)
        : m_light(light)
    {}

    LightComponent::~LightComponent()
    {}

    void LightComponent::Update(const GameTimer & gameTimer)
    {}

    void LightComponent::Draw(const GameTimer & gameTimer)
    {}

    Spectrum LightComponent::Sample_Li(const SurfaceInfo & surface_w, const Vector2f & sample, Point3f * endpoint, float* pdf) const
    {
        auto& transform = m_attachedObject->GetComponent<TransformComponent>()->GetTransform();
        return m_light->Sample_Li(surface_w, transform, sample, endpoint, pdf);
    }
}
