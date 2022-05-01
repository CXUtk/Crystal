#include "Sphere.h"
#include <glm/gtx/transform.hpp>
#include <Core/Sampling/Sampling.h>

namespace crystal
{

    //std::shared_ptr<Shape> Sphere::CreateSphere(JsonNode_CPTR pShapeNode)
    //{
    //    auto pos = loader::parse_vec3(pShapeNode->GetMember("Position"));
    //    auto r = pShapeNode->GetMember("Radius")->GetFloat();

    //    Vector3f rotation = glm::vec3(0);
    //    if (pShapeNode->HasMember("Rotation"))
    //    {
    //        rotation = loader::parse_vec3(pShapeNode->GetMember("Rotation"));
    //    }
    //    return std::make_shared<Sphere>(pos, r, rotation);
    //}

    Sphere::Sphere(float radius, const Transform& transform)
        : m_radius(radius)
    {
        m_position = transform.GetTranslation();
        Matrix4f rotMatrix = glm::toMat4(transform.GetRotation());
        m_local2World = glm::mat3(rotMatrix);
        m_world2Local = glm::transpose(m_local2World);
    }

    Sphere::~Sphere()
    {}

    Bound3f Sphere::GetBoundingBox() const
    {
        return Bound3f(glm::vec3(m_position - m_radius), glm::vec3(m_position + m_radius));
    }

    bool Sphere::Intersect(const Ray3f& ray, SurfaceInteraction* info) const
    {
        Point3f P = m_world2Local * (ray.Start() - m_position);
        Vector3f d = m_world2Local * ray.Dir();
        float a = glm::dot(d, d);
        float b = 2 * glm::dot(d, P);
        float c = glm::dot(P, P) - m_radius * m_radius;
        float discrim = b * b - 4 * a * c;
        if (discrim < 0) return false;
        discrim = sqrt(discrim);
        float t1 = (-b + discrim) / (2 * a);
        float t2 = (-b - discrim) / (2 * a);
        if (t1 > t2) std::swap(t1, t2);
        float t = t1;
        if (std::isnan(t1) || t1 < 0) t = t2;
        if (std::isnan(t) || t < 0) return false;

        // Calculate local hit info, normal, front face, etc..
        auto dummyHitPos = P + d * t;

        auto N = glm::normalize(dummyHitPos);
        auto phi = (std::atan2(-dummyHitPos.z, dummyHitPos.x) + glm::pi<float>()) / glm::pi<float>();
        auto theta = std::acos(dummyHitPos.y / m_radius) / glm::pi<float>() * 0.5f + 0.5f;

        auto front_face = glm::dot(d, N) < 0;
        N = front_face ? N : -N;

        auto realHitPos = m_local2World * dummyHitPos + m_position;
        auto dpdu = glm::normalize(m_local2World * Vector3f(-N.z, 0, N.x));
        N = m_local2World * N;
        // Interaction normal
        info->SetHitInfo(t, realHitPos, ray.Dir(), N, Point2f(phi * 0.5f, theta), front_face, dpdu, glm::cross(N, dpdu));

        if (std::isinf(t) || std::isnan(t))
        {
            printf("Invalid distance on Sphere intersection: %.lf\n", t);
            throw;
        }
        return true;
    }

    bool Sphere::IntersectTest(const Ray3f& ray, float tMin, float tMax) const
    {
        if (tMin > tMax) return false;
        glm::vec3 P = m_world2Local * (ray.Start() - m_position);
        glm::vec3 d = m_world2Local * ray.Dir();
        float a = glm::dot(d, d);
        float b = 2 * glm::dot(d, P);
        float c = glm::dot(P, P) - m_radius * m_radius;
        float discrim = b * b - 4 * a * c;
        if (discrim < 0) return false;
        discrim = sqrt(discrim);
        float t1 = (-b + discrim) / (2 * a);
        float t2 = (-b - discrim) / (2 * a);
        return (t1 >= tMin && t1 <= tMax) || (t2 >= tMin && t2 <= tMax);
    }

    float Sphere::SurfaceArea() const
    {
        return 4 * glm::pi<float>() * m_radius * m_radius;
    }

    SurfaceInfo Sphere::SampleSurface(const Vector2f& sample) const
    {
        float p;
        auto dir = NextUnitSphere(sample, &p);
        return SurfaceInfo(m_position + dir * m_radius, dir);
    }

    SurfaceInfo Sphere::SampleSurfaceLight(const Vector2f& sample, const SurfaceInteraction& ref) const
    {
        return SampleSurface(sample);
    }
}