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
        Vector3f GetForwardDir() const;
        Vector3f GetUpDir() const;
        Vector3f GetRightDir() const;
        Float GetAspectRatio() const { return m_AspectRatio; }
        Float GetFOV() const { return m_FovY; }


        Matrix4f GetProjectionMatrix() const;
        Matrix4f GetViewMatrix() const;

        Vector3f GetDirection(const Vector2f& coord) const;

    private:
        const Transform*    m_tranform = nullptr;
        Float               m_FovY{}, m_AspectRatio{}, m_zNear{}, m_zFar{};
    };
}