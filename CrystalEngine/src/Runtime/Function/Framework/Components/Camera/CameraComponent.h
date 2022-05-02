#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Component.h"
#include "Camera.h"

namespace crystal
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent(const SJson::JsonNode& setting);
        virtual ~CameraComponent() override;

        virtual void Initialize() override;
        virtual void Update(const GameTimer& gameTimer) override;
        virtual void Draw(const GameTimer& gameTimer) override;

        std::shared_ptr<Camera> GetCamera() const { return m_camera; }
        Ray3f GenerateRay(const Vector2f& coord) const;

    private:
        SJson::JsonNode         m_setting{};
        std::shared_ptr<Camera> m_camera = nullptr;
    };
}