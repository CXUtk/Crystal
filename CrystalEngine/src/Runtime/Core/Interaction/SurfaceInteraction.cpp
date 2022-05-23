#include "SurfaceInteraction.h"
#include <Function/Framework/Components/Light/LightComponent.h>
#include <Function/Framework/Components/Light/Lights/AreaLight.h>
#include <Function/Framework/Components/Medium/Medium.h>


namespace crystal
{
    static constexpr float EPS = 1e-4f;

    SurfaceInteraction::~SurfaceInteraction()
    {}

    RayTr SurfaceInteraction::SpawnRay(const Vector3f& dir) const
    {
        auto interactionNormal = GetInteractionNormal();
        bool back = glm::dot(dir, interactionNormal) < 0;
        RayTr ray = {};
        ray.Ray = Ray(m_position + ((back) ? -interactionNormal : interactionNormal) * EPS, dir);
        ray.Medium = GetMedium(dir);
        return ray;
    }

    RayTr SurfaceInteraction::SpawnRayTo(const Point3f& pos) const
    {
        auto interactionNormal = GetInteractionNormal();
        bool back = glm::dot(pos - m_position, interactionNormal) < 0;
        Point3f origin = m_position + ((back) ? -interactionNormal : interactionNormal) * EPS;
        RayTr ray = {};
        ray.Ray = Ray(origin, pos - origin);
        ray.Medium = GetMedium(pos - origin);
        return ray;
    }

    void SurfaceInteraction::SetHitInfo(float t, const Point3f& hitPos, const Vector3f& wo,
        const Vector3f& modelNormal, const Vector2f& uv, bool frontFace,
        const Vector3f& dpdu, const Vector3f& dpdv)
    {
        m_position = hitPos;
        m_distance = t;
        m_wo = wo;
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

    Spectrum SurfaceInteraction::Le(const Vector3f& w) const
    {
        auto light = m_primitive->GetAreaLight();
        return light ? light->Eval_Le(GetGeometryInfo(), w) : Spectrum(0.f);
    }

    const Medium* SurfaceInteraction::GetMedium(const Vector3f wi) const
    {
        bool front = glm::dot(wi, m_modelNormal) > 0;
        return front ? m_mediumInterface.Outside : m_mediumInterface.Inside;
    }

    SurfaceGeometryInfo SurfaceInteraction::GetGeometryInfo() const
    {
        return SurfaceGeometryInfo(m_position, m_modelNormal);
    }

    Ray3f SurfaceGeometryInfo::SpawnRay(const Vector3f& dir) const
    {
        bool back = glm::dot(dir, m_normal) < 0;
        auto start = m_position + ((back) ? -m_normal : m_normal) * EPS;
        return Ray3f(start, dir);
    }

    RayTr InteractionInfo::SpawnRay(const Vector3f& dir) const
    {
        RayTr ray = {};
        ray.Ray = Ray3f(m_position, dir);
        ray.Medium = GetMedium(dir);
        return ray;
    }
    RayTr InteractionInfo::SpawnRayTo(const Point3f& pos) const
    {
        RayTr ray = {};
        ray.Ray = Ray3f(m_position, pos - m_position);
        ray.Medium = GetMedium(pos - m_position);
        return ray;
    }
    Vector3f InteractionInfo::ToLocalCoordinate(const Vector3f& v) const
    {
        return v;
    }
    Vector3f InteractionInfo::ToWorldCoordinate(const Vector3f& v) const
    {
        return v;
    }
    const Medium* InteractionInfo::GetMedium(const Vector3f wi) const
    {
        return nullptr;
    }

    MediumInteractionInfo::~MediumInteractionInfo()
    {}
    Vector3f MediumInteractionInfo::ToLocalCoordinate(const Vector3f & v) const
    {
        Vector3f newV = m_InvTNB * v;
        FixVector(newV);
        return newV;
    }
    Vector3f MediumInteractionInfo::ToWorldCoordinate(const Vector3f& v) const
    {
        Vector3f newV = m_TNB * v;
        FixVector(newV);
        return newV;
    }
}