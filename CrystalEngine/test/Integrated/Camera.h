#pragma once
#include <Crystal.h>
#include <Interfaces/Interfaces.h>

using namespace crystal;

class Camera {
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

private:
    Point3f     m_EyePos, m_LookAt;
    Vector3f    m_Up;
    Float       m_FovY, m_AspectRatio, m_zNear, m_zFar;
};