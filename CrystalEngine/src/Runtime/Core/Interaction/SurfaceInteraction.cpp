#include "SurfaceInteraction.h"
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>


namespace crystal
{
    static constexpr float EPS = 1e-4f;

    Ray3f SurfaceInfo::SpawnRay(const Vector3f& dir) const
    {
        return Ray3f(m_position + m_normal * EPS, dir);
    }

    SurfaceInteraction::SurfaceInteraction()
    {
    }

    Ray3f SurfaceInteraction::SpawnRay(const Vector3f& dir) const
    {
        bool back = glm::dot(dir, m_normal) < 0;
        return Ray(m_hitPos + ((back) ? -m_normal : m_normal) * EPS, dir);
    }

    Ray3f SurfaceInteraction::SpawnRayTo(const Point3f& pos) const
    {
        bool back = glm::dot(pos - m_hitPos, m_normal) < 0;
        Point3f origin = m_hitPos + ((back) ? -m_normal : m_normal) * EPS;
        return Ray(origin, pos - origin);
    }

    void SurfaceInteraction::SetHitInfo(float t, const Point3f& hitPos, const Vector3f& hitDir, const Vector3f& normal,
        const Vector2f& uv, bool frontFace, const Vector3f& dpdu, const Vector3f& dpdv)
    {
        m_hitPos = hitPos;
        m_distance = t;
        m_hitDir = hitDir;
        m_texCoord = uv;
        m_normal = normal;
        m_isFrontFace = frontFace;
        m_dpDu = dpdu;
        m_dpDv = dpdv;
    }


    Matrix3f SurfaceInteraction::GetTNB() const
    {
        return glm::mat3(m_dpDu, m_normal, m_dpDv);
    }

    Matrix3f SurfaceInteraction::GetInverseTNB() const
    {
        return glm::transpose(GetTNB());
    }

    SurfaceInfo SurfaceInteraction::GetSurfaceInfo(bool modelNormal) const
    {
        return SurfaceInfo(m_hitPos, modelNormal ? (m_isFrontFace ? m_normal : -m_normal) : m_normal);
    }

    Spectrum SurfaceInteraction::Le(const Vector3f& w) const
    {
        auto light = m_primitive->GetAreaLight();
        return light ? light->Eval_Le(GetSurfaceInfo(true), w) : Spectrum(0.f);
    }
}