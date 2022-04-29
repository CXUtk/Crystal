#include "Camera.h"
namespace crystal
{
    Camera::Camera(Float fovY, Float aspect, Float zNear, Float zFar) : m_FovY(fovY),
        m_AspectRatio(aspect), m_zNear(zNear), m_zFar(zFar)
    {


    }

    Camera::~Camera()
    {}

    //void Camera::SetEyePos(Point3f eye)
    //{
    //    m_EyePos = eye;
    //}

    //void Camera::SetLookAt(Point3f pos)
    //{
    //    m_LookAt = pos;
    //}

    void Camera::SetAspectRatio(Float aspect)
    {
        m_AspectRatio = aspect;
    }

    //Point3f Camera::GetEyePos() const
    //{
    //    return m_EyePos;
    //}

    //Point3f Camera::GetLookAt() const
    //{
    //    return m_LookAt;
    //}

    Matrix4f Camera::GetProjectionMatrix() const
    {
        return crystal::crystal_perspective(m_FovY, m_AspectRatio, m_zNear, m_zFar);
    }

    //Matrix4f Camera::GetViewMatrix() const
    //{
    //    return crystal::crystal_look_at(m_EyePos, m_LookAt, m_Up);
    //}

    Vector3f Camera::GetDirection(const Vector2f& coord, const Transform& transform) const
    {
        auto T = transform.getMatrix();
        auto forward = T * Vector4f(0, 0, -1, 0);
        auto up = T * Vector4f(0, 1, 0, 0);
        auto right = T * Vector4f(1, 0, 0, 0);

        float upLength = std::tan(m_FovY / 2.f);
        float rightLength = upLength * m_AspectRatio;
        Vector2f uv = coord * 2.f - 1.f;
        Vector3f dir = Vector3f(1.f, uv.y * upLength, uv.x * rightLength);
        dir = dir.x * forward + dir.y * up + dir.z * right;
        return glm::normalize(dir);
    }
}
