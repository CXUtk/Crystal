#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SJson/SJson.hpp>

#include "../Component.h"
#include "Lights/Light.h"

namespace crystal
{
    class LightComponent : public Component
    {
    public:
        LightComponent(const SJson::JsonNode& setting);
        virtual ~LightComponent() override;

        virtual void Initialize() override;
        virtual void Update(const GameTimer& gameTimer) override;
        virtual void Draw(const GameTimer& gameTimer) override;

        const std::vector<std::shared_ptr<Light>>& GetLights() const { return m_lights; }
    private:
        SJson::JsonNode                     m_setting{};
        std::vector<std::shared_ptr<Light>> m_lights{};
    };
}