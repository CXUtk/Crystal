#pragma once
#include <Crystal.h>
#include <Platform/RHI/Interfaces.h>
#include <Core/Math/Transform.h>

namespace crystal
{
    class Camera
    {
    public:
        Camera(Float fovY, Float aspect, Float zNear, Float zFar);
        ~Camera();

        void SetEyePos(Point3f eye);
        void SetLookAt(Point3f pos);
        void SetAspectRatio(Float aspect);

        Point3f GetEyePos() const;
        Point3f GetLookAt() const;

        Matrix4f GetProjectionMatrix() const;
        Matrix4f GetViewMatrix() const;

        Vector3f GetDirection(const Vector2f& coord, const Transform& transform) const;

    private:
        Float       m_FovY{}, m_AspectRatio{}, m_zNear{}, m_zFar{};
    };
}