#include "SurfaceInteraction.h"
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>
#include <Function/Framework/Components/Medium/Medium.h>


namespace crystal
{
    static constexpr float EPS = 1e-4f;

    SurfaceInteraction::SurfaceInteraction()
    {
    }

    RayTr SurfaceInteraction::SpawnRay(const Vector3f& dir) const
    {
        auto interactionNormal = GetInteractionNormal();
        bool back = glm::dot(dir, interactionNormal) < 0;
        RayTr ray = {};
        ray.Ray = Ray(m_hitPos + ((back) ? -interactionNormal : interactionNormal) * EPS, dir);
        ray.Medium = GetMedium(dir);
        return ray;
    }

    RayTr SurfaceInteraction::SpawnRayTo(const Point3f& pos) const
    {
        auto interactionNormal = GetInteractionNormal();
        bool back = glm::dot(pos - m_hitPos, interactionNormal) < 0;
        Point3f origin = m_hitPos + ((back) ? -interactionNormal : interactionNormal) * EPS;
        RayTr ray = {};
        ray.Ray = Ray(origin, pos - origin);
        ray.Medium = GetMedium(pos - origin);
        return ray;
    }

    void SurfaceInteraction::SetHitInfo(float t, const Point3f& hitPos, const Vector3f& hitDir,
        const Vector3f& modelNormal, const Vector2f& uv, bool frontFace,
        const Vector3f& dpdu, const Vector3f& dpdv)
    {
        m_hitPos = hitPos;
        m_distance = t;
        m_hitDir = hitDir;
        m_texCoord = uv;
        m_modelNormal = modelNormal;
        m_isFrontFace = frontFace;
        m_dpDu = dpdu;
        m_dpDv = dpdv;

        m_TNB = Matrix3f(m_dpDu, modelNormal, m_dpDv);
        m_invTNB = glm::transpose(m_TNB);
    }


    Matrix3f SurfaceInteraction::GetInteractionTNB() const
    {
        return m_isFrontFace ? m_TNB : -m_TNB;
    }

    Matrix3f SurfaceInteraction::GetInteractionInverseTNB() const
    {
        return m_isFrontFace ? m_invTNB : -m_invTNB; 
    }

    Vector3f SurfaceInteraction::ToLocalCoordinate(const Vector3f& v) const
    {
        Vector3f newV = GetInteractionInverseTNB() * v;
        FixVector(newV);
        return newV;
    }

    Vector3f SurfaceInteraction::ToWorldCoordinate(const Vector3f& v) const
    {
        Vector3f newV = GetInteractionTNB() * v;
        FixVector(newV);
        return newV;
    }


    InteractionGeometryInfo SurfaceInteraction::GetGeometryInfo(bool modelNormal) const
    {
        return InteractionGeometryInfo(m_hitPos, modelNormal ? m_modelNormal : GetInteractionNormal());
    }

    Spectrum SurfaceInteraction::Le(const Vector3f& w) const
    {
        auto light = m_primitive->GetAreaLight();
        return light ? light->Eval_Le(GetGeometryInfo(true), w) : Spectrum(0.f);
    }

    const Medium* SurfaceInteraction::GetMedium(const Vector3f wi) const
    {
        bool front = glm::dot(wi, m_modelNormal) > 0;
        return front ? m_mediumInterface.Outside : m_mediumInterface.Inside;
    }

    Ray3f InteractionGeometryInfo::SpawnRay(const Vector3f& dir) const
    {
        bool back = glm::dot(dir, m_normal) < 0;
        auto start = m_position + ((back) ? -m_normal : m_normal) * EPS;
        return Ray3f(start, dir);
    }

    RayTr InteractionInfo::SpawnRayTr(const Vector3f& dir) const
    {
        bool back = glm::dot(dir, GetNormal()) < 0;
        RayTr ray = {};
        auto start = GetPosition() + ((back) ? -GetNormal() : GetNormal()) * EPS;
        ray.Ray = Ray3f(start, dir);
        ray.Medium = back ? m_mediumInterface.Inside : m_mediumInterface.Outside;
        return ray;
    }
    RayTr MediumInteractionInfo::SpawnRayTr(const Vector3f& dir) const
    {
        RayTr ray = {};
        ray.Ray = Ray3f(m_position, dir);
        ray.Medium = m_medium;
        return ray;
    }
    RayTr MediumInteractionInfo::SpawnRayTrTo(const Point3f& pos) const
    {
        RayTr ray = {};
        ray.Ray = Ray3f(m_position, pos - m_position);
        ray.Medium = m_medium;
        return ray;
    }
    InteractionGeometryInfo MediumInteractionInfo::GetGeometryInfo(bool modelNormal) const
    {
        return InteractionGeometryInfo(m_position, Vector3f(0, 1, 0));
    }
}