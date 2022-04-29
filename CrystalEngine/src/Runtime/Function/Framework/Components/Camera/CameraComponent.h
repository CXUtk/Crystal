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
        CameraComponent(std::shared_ptr<Camera> camera);
        virtual ~CameraComponent() override;

        virtual void Update(const GameTimer& gameTimer) override;
        virtual void Draw(const GameTimer& gameTimer) override;

        std::shared_ptr<Camera> GetCamera() const { return m_camera; }
        Ray3f GenerateRay(const Vector2f& coord) const;

    private:
        std::shared_ptr<Camera>  m_camera = nullptr;
    };
}