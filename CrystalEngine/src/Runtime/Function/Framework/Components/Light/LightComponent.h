#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include "Light.h"

namespace crystal
{
    class LightComponent : public Component
    {
    public:
        LightComponent();
        virtual ~LightComponent() override;

        virtual void Update(const GameTimer& gameTimer);
        virtual void Draw(const GameTimer& gameTimer);

    private:
        std::shared_ptr<Light>  m_light = nullptr;
    };
}