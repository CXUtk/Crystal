#pragma once
#include <Crystal.h>
#include <Platform/RHI/Interfaces.h>
#include <Core/Math/Transform.h>

#include "ComponentSettings.h"

namespace crystal
{
    class Camera
    {
    public:
        Camera(const Transform* transform, const PerspectiveCameraSetting& setting);
        ~Camera();

        Point3f GetEyePos() const;
        Point3f GetForwardDir() const;

        Matrix4f GetProjectionMatrix() const;
        Matrix4f GetViewMatrix() const;

        Vector3f GetDirection(const Vector2f& coord) const;

    private:
        const Transform*    m_tranform = nullptr;
        Float               m_FovY{}, m_AspectRatio{}, m_zNear{}, m_zFar{};
    };
}