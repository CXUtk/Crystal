#pragma once

#include "Math.h"
#include "Geometry.h"
#include <glm/gtx/transform.hpp>
#include <SRefl/SRefl.hpp>

namespace crystal
{
    class Transform
    {
        SREFL_TYPE_HEADER(Transform);
    public:
        Transform() {}
        Transform(const Vector3f& translate, const Vector3f& scale, const Quaternion& rotation)
            : m_translation(translate), m_scale(scale), m_rotation(rotation)
        {

        }

        ~Transform() {}

        Matrix4f GetMatrix() const
        {
            Matrix4f mat = glm::identity<Matrix4f>();
            if (m_scale != Vector3f(1.f))
            {
                mat = glm::scale(m_scale) * mat;
            }
            if (m_rotation != glm::identity<Quaternion>())
            {
                mat = glm::toMat4(m_rotation) * mat;
            }
            mat = glm::translate(m_translation) * mat;
            return mat;
        }

        void SetTranslation(const Vector3f& translation) { m_translation = translation; }
        void SetScale(const Vector3f& scale) { m_scale = scale; }
        void SetRotation(const Quaternion& quat) { m_rotation = quat; }

        Vector3f GetTranslation() const { return m_translation; }
        Vector3f GetScale() const { return m_scale; }
        Quaternion GetRotation() const { return m_rotation; }

    private:
        Vector3f m_translation{ 0.f };
        Vector3f m_scale{ 1.f };
        Quaternion m_rotation = glm::identity<Quaternion>();
    };
}

template<>
struct SRefl::TypeInfo<crystal::Transform>
{
    SREFL_TYPEINFO_HEADER(crystal::Transform);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SRefl::_field(&_Type::m_translation, "Translation"),
            SRefl::_field(&_Type::m_scale, "Scale"),
            SRefl::_field(&_Type::m_rotation, "Rotation")
        );
    }
};