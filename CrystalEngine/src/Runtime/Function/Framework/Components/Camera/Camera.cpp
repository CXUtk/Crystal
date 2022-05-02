#include "Camera.h"
namespace crystal
{
    static const Vector3f Forward(0, 0, -1);
    static const Vector3f Up(0, 1, 0);
    static const Vector3f Right(1, 0, 0);

    Camera::Camera(const Transform* transform, const PerspectiveCameraSetting& setting)
        : m_tranform(transform), m_FovY(setting.FovY),
        m_AspectRatio(setting.Aspect), m_zNear(setting.ZNear), m_zFar(setting.ZFar)
    {
    }

    Camera::~Camera()
    {}

    Point3f Camera::GetEyePos() const
    {
        return m_tranform->GetTranslation();
    }

    Point3f Camera::GetForwardDir() const
    {
        return m_tranform->GetRotation() * Forward;
    }

    Matrix4f Camera::GetProjectionMatrix() const
    {
        return crystal::crystal_perspective(m_FovY, m_AspectRatio, m_zNear, m_zFar);
    }

    Matrix4f Camera::GetViewMatrix() const
    {
        auto pos = m_tranform->GetTranslation();
        return crystal::crystal_look_at(pos, pos + GetForwardDir(), m_tranform->GetRotation() * Up);
    }

    Vector3f Camera::GetDirection(const Vector2f& coord) const
    {
        auto T = m_tranform->getMatrix();
        auto forward = T * Vector4f(Forward, 0);
        auto up = T * Vector4f(Up, 0);
        auto right = T * Vector4f(Right, 0);

        float upLength = std::tan(m_FovY / 2.f);
        float rightLength = upLength * m_AspectRatio;
        Vector2f uv = coord * 2.f - 1.f;
        Vector3f dir = Vector3f(1.f, uv.y * upLength, uv.x * rightLength);
        dir = dir.x * forward + dir.y * up + dir.z * right;
        return glm::normalize(dir);
    }
}
