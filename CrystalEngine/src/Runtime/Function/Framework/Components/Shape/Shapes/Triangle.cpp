#include "Triangle.h"
#include <glm/gtx/transform.hpp>
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    constexpr bool FLAT_SHADING = false;

    glm::vec3 bary_interp(glm::vec3 bary, glm::vec3 A, glm::vec3 B, glm::vec3 C)
    {
        return bary.x * A + bary.y * B + bary.z * C;
    }
    glm::vec2 bary_interp(glm::vec3 bary, glm::vec2 A, glm::vec2 B, glm::vec2 C)
    {
        return bary.x * A + bary.y * B + bary.z * C;
    }

    Triangle::Triangle(const MeshVertexData* a, const MeshVertexData* b, const MeshVertexData* c)
    {
        m_vertices[0] = a, m_vertices[1] = b, m_vertices[2] = c;
        calculateDerivative();
    }

    Triangle::~Triangle()
    {}

    Bound3f Triangle::GetBoundingBox() const
    {
        return m_bBox;
    }

    bool Triangle::Intersect(const Ray3f& ray, SurfaceInteraction* isec) const
    {
        glm::mat3 A(m_vertices[1]->Position - m_vertices[0]->Position,
            m_vertices[2]->Position - m_vertices[0]->Position, -ray.Dir());
        glm::vec3 P = ray.Start() - m_vertices[0]->Position;
        glm::vec3 res;
        // 通过求解 Ax = b 算出光线打在三角形上的重心坐标
        res = glm::inverse(A) * P;
        if (glm::isnan(res) != glm::bvec3(false)) return false;
        if (res.x < 0.f || res.x > 1.f || res.y < 0.f || res.y > 1.f || res.x + res.y > 1.0001f || res.z < 0.f) return false;
        glm::vec3 bary_coord = glm::vec3(1 - res.x - res.y, res.x, res.y);
        glm::vec3 N;
        glm::vec2 UV = bary_interp(bary_coord, m_vertices[0]->TexCoord, m_vertices[1]->TexCoord, m_vertices[2]->TexCoord);
        if (FLAT_SHADING)
        {
            N = glm::normalize(glm::cross(m_vertices[1]->Position - m_vertices[0]->Position,
                m_vertices[2]->Position - m_vertices[1]->Position));
        }
        else
        {
            N = glm::normalize(bary_interp(bary_coord, m_vertices[0]->Normal,
                m_vertices[1]->Normal, m_vertices[2]->Normal));
        }
        auto front_face = glm::dot(ray.Dir(), N) < 0;
        N = front_face ? N : -N;

        auto dpdu = m_dpDu;
        // 如果没有切线向量（通常是因为没有纹理坐标绑定），那么就手动计算一个
        if (glm::isnan(dpdu) != glm::bvec3(false))
        {
            for (int i = 0; i < 3; i++)
            {
                Vector3f v(0);
                v[i] = 1;
                auto tmp = glm::cross(v, N);
                if (tmp != Vector3f(0))
                {
                    dpdu = glm::normalize(tmp);
                    break;
                }
            }
        }

        isec->SetHitInfo(res.z, ray.Start() + ray.Dir() * res.z, ray.Dir(), N, UV, front_face, dpdu, glm::cross(N, dpdu));
        if (std::isinf(res.z) || std::isnan(res.z))
        {
            printf("Invalid distance on triangle intersection: %lf\n", res.z);
            throw;
        }
        return true;
    }

    bool Triangle::IntersectTest(const Ray3f& ray, Float* t, float tMin, float tMax) const
    {
        if (tMin > tMax) return false;
        glm::mat3 A(m_vertices[1]->Position - m_vertices[0]->Position,
            m_vertices[2]->Position - m_vertices[0]->Position,
            -ray.Dir());
        glm::vec3 P = ray.Start() - m_vertices[0]->Position;
        auto res = glm::inverse(A) * P;
        if (glm::isnan(res) != glm::bvec3(false)) return false;
        if (res.x < 0 || res.x > 1 || res.y < 0 || res.y > 1 || res.x + res.y > 1 || res.z < 0) return false;
        *t = res.z;
        return res.z >= tMin && res.z <= tMax;
    }

    float Triangle::SurfaceArea() const
    {
        return glm::length(glm::cross(m_vertices[1]->Position - m_vertices[0]->Position,
            m_vertices[2]->Position - m_vertices[0]->Position)) / 2.f;
    }

    SurfaceInfo Triangle::SampleSurface(const Vector2f& sample) const
    {
        Vector3f baryCoord = sampleTriangle(sample);
        Point3f pos = baryCoord.x * m_vertices[0]->Position
            + baryCoord.y * m_vertices[1]->Position
            + baryCoord.z * m_vertices[2]->Position;
        Normal3f N;
        if (FLAT_SHADING)
        {
            N = glm::normalize(glm::cross(m_vertices[1]->Position - m_vertices[0]->Position,
                m_vertices[2]->Position - m_vertices[1]->Position));
        }
        else
        {
            N = glm::normalize(bary_interp(baryCoord, m_vertices[0]->Normal,
                m_vertices[1]->Normal, m_vertices[2]->Normal));
        }
        return SurfaceInfo(pos, N);
    }

    Vector3f Triangle::sampleTriangle(const Vector2f& sample) const
    {
        auto u = 1 - std::sqrt(sample.x);
        auto v = sample.y * std::sqrt(sample.x);
        return Vector3f(1.f - u - v, u, v);
    }


    void Triangle::calculateDerivative()
    {
        // 通过纹理坐标算出UV方向的切线向量
        Matrix2f A(
            Vector2f(m_vertices[0]->TexCoord.x - m_vertices[2]->TexCoord.x,
                m_vertices[0]->TexCoord.y - m_vertices[2]->TexCoord.y),
            Vector2f(m_vertices[1]->TexCoord.x - m_vertices[2]->TexCoord.x,
                m_vertices[1]->TexCoord.y - m_vertices[2]->TexCoord.y)
        );
        A = glm::inverse(A);

        if (glm::isinf(A[0]) != glm::bvec2(false)
            || glm::isnan(A[0]) != glm::bvec2(false)
            || glm::isnan(A[1]) != glm::bvec2(false))
        {
            m_dpDu = m_dpDv = glm::vec3(NAN);
        }
        else
        {
            auto a = m_vertices[0]->Position - m_vertices[2]->Position;
            auto b = m_vertices[1]->Position - m_vertices[2]->Position;
            auto res = A * glm::mat3x2(glm::vec2(a.x, b.x), glm::vec2(a.y, b.y), glm::vec2(a.z, b.z));
            m_dpDu = glm::normalize(glm::vec3(res[0][0], res[1][0], res[2][0]));
            m_dpDv = glm::normalize(glm::vec3(res[0][1], res[1][1], res[2][1]));
        }

        glm::vec3 minn = m_vertices[0]->Position, maxx = m_vertices[0]->Position;
        for (int i = 1; i < 3; i++)
        {
            minn.x = std::min(minn.x, m_vertices[i]->Position.x);
            minn.y = std::min(minn.y, m_vertices[i]->Position.y);
            minn.z = std::min(minn.z, m_vertices[i]->Position.z);

            maxx.x = std::max(maxx.x, m_vertices[i]->Position.x);
            maxx.y = std::max(maxx.y, m_vertices[i]->Position.y);
            maxx.z = std::max(maxx.z, m_vertices[i]->Position.z);
        }
        m_bBox = BoundingBox(minn, maxx);
    }
}