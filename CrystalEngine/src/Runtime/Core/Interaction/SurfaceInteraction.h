#pragma once
#include <memory>
#include <Core/Math/Geometry.h>
#include <Function/Framework/Object/GameObject.h>
#include <Platform/RHI/Graphics/GraphicsCommon.h>

namespace crystal
{
    // Stores information of current surface patch (with front direction)
    class SurfaceInfo
    {
    public:
        SurfaceInfo(const Point3f& position, const Normal3f& normal)
            : m_position(position), m_normal(normal) {}
        Ray3f SpawnRay(const Vector3f& dir) const;

        Point3f GetPosition() const { return m_position; }
        Normal3f GetNormal() const { return m_normal; }
    private:
        Point3f m_position;
        Normal3f m_normal;
    };

    class SurfaceInteraction
    {
    public:
        SurfaceInteraction();

        // Spawn a ray from its current hit information
        Ray3f SpawnRay(const Vector3f& dir) const;
        Ray3f SpawnRayTo(const Point3f& pos) const;

        void SetHitInfo(float t, const Point3f& hitPos, const Vector3f& hitDir, const Vector3f& normal,
            const Vector2f& uv, bool frontFace, const Vector3f& dpdu, const Vector3f& dpdv);

        bool IsFrontFace() const { return m_isFrontFace; }
        float GetDistance() const { return m_distance; }

        void SetHitObject(const GameObject* object) { m_hitObject = object; }
        const GameObject* GetHitObject() const { return m_hitObject; }

        void SetHitPos(const Point3f& pos) { m_hitPos = pos; }
        Point3f GetHitPos() const { return m_hitPos; }

        void SetNormal(const Normal3f& normal) { m_normal = normal; }
        Normal3f GetNormal() const { return m_normal; }

        void SetTexCoord(const Point2f& texCoord) { m_texCoord = texCoord; }
        Point2f GetTexCoord() const { return m_texCoord; }

        void SetDpDu(const Vector3f& dir) { m_dpDu = dir; }
        Vector3f GetDpDu() const { return m_dpDu; }

        void SetDpDv(const Vector3f& dir) { m_dpDv = dir; }
        Vector3f GetDpDv() const { return m_dpDv; }

        void SetHitDir(const Vector3f& dir) { m_hitDir = dir; }
        Vector3f GetHitDir() const { return m_hitDir; }


        glm::mat3 GetTNB() const;
        glm::mat3 GetInverseTNB() const;

        SurfaceInfo GetSurfaceInfo(bool modelNormal) const;

        /**
         * @brief Get the emitted radiance at a surface point intersected by a ray
         * @param w Direction of the ray in world coord
         * @return
        */
        Spectrum Le(const Vector3f& w) const;

        void SetBSDF(std::shared_ptr<)

    private:
        const GameObject*   m_hitObject = nullptr;
        float               m_distance = std::numeric_limits<float>::infinity();
        bool                m_isFrontFace = false;
        Point3f             m_hitPos{};
        Vector3f            m_hitDir{},
                            m_dpDu{},
                            m_dpDv{};
        Normal3f            m_normal{};
        Vector2f            m_texCoord{};
    };
}