#include "Camera.h"

Camera::Camera(Float fovY, Float aspect, Float zNear, Float zFar) : m_FovY(fovY), 
    m_AspectRatio(aspect), 
    m_zNear(zNear), m_zFar(zFar) {
    m_EyePos = glm::vec3(0, 0, 5);
    m_LookAt = glm::vec3(0, 0, 0);
    m_Up = glm::vec3(0, 1, 0);
}

Camera::~Camera() {
}

void Camera::SetEyePos(Point3f eye)
{
    m_EyePos = eye;
}

void Camera::SetLookAt(Point3f pos)
{
    m_LookAt = pos;
}

void Camera::SetAspectRatio(Float aspect)
{
    m_AspectRatio = aspect;
}

Point3f Camera::GetEyePos() const
{
    return m_EyePos;
}

Point3f Camera::GetLookAt() const
{
    return m_LookAt;
}

Matrix4f Camera::GetProjectionMatrix() const
{
    return crystal::crystal_perspective(m_FovY, m_AspectRatio, m_zNear, m_zFar);
}

Matrix4f Camera::GetViewMatrix() const
{
    return crystal::crystal_look_at(m_EyePos, m_LookAt, m_Up);
}
