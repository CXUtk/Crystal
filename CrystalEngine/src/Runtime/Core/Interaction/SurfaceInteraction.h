#pragma once
#include <memory>
#include <Platform/RHI/Graphics/GraphicsCommon.h>

#include <Core/Math/Geometry.h>
#include <Core/Interaction/RayTr.h>

#include <Function/Framework/Object/GameObject.h>
#include <Function/Framework/Components/Material/BSDF.h>
#include <Function/Framework/Components/Material/Material.h>
#include <Function/Framework/Components/Material/MaterialComponent.h>
#include <Function/Framework/Components/Medium/Medium.h>
#include <Function/Framework/Interfaces/IRayPrimitive.h>
#include <Function/Framework/Components/Medium/PhaseFunctions/PhaseFunction.h>

namespace crystal
{
    // Stores information of current surface patch (with front direction)
    class SurfaceGeometryInfo
    {
    public:
        SurfaceGeometryInfo(const Point3f& position, const Normal3f& normal)
            : m_position(position), m_normal(normal)
        {}
        Ray3f SpawnRay(const Vector3f& dir) const;
        Point3f GetPosition() const { return m_position; }
        Normal3f GetNormal() const { return m_normal; }

    private:
        Point3f m_position{};
        Normal3f m_normal{};
    };

    // Stores information of simplified current interaction event
    class InteractionInfo
    {
    public:
        InteractionInfo() {}
        InteractionInfo(const Point3f& position) : m_position(position) {}
        InteractionInfo(const Point3f& position, const Vector3f& wo, const MediumInterface& minterface)
            : m_position(position), m_wo(wo), m_mediumInterface(minterface) {}
        virtual ~InteractionInfo() = 0 {};

        virtual RayTr SpawnRay(const Vector3f& dir) const;
        virtual RayTr SpawnRayTo(const Point3f& pos) const;
        virtual Vector3f ToLocalCoordinate(const Vector3f& v) const;
        virtual Vector3f ToWorldCoordinate(const Vector3f& v) const;
        // Get the medium on given exit/incoming direction
        virtual const Medium* GetMedium(const Vector3f wi) const;

        Point3f GetPosition() const { return m_position; }
        Vector3f GetW_Out() const { return m_wo; }

        virtual bool IsSurfaceInteraction() const = 0;

        virtual SurfaceGeometryInfo GetGeometryInfo() const { return SurfaceGeometryInfo(m_position, -m_wo); }

    protected:
        Point3f             m_position{};
        Vector3f            m_wo{};
        MediumInterface     m_mediumInterface{};
    };



    // Stores information of current surface patch (with front direction)
    class MediumInteractionInfo : public InteractionInfo
    {
    public:
        MediumInteractionInfo() : InteractionInfo() {}
        MediumInteractionInfo(const Point3f& position,
            const Vector3f& wo,
            const Medium* medium)
            : InteractionInfo(position, wo, MediumInterface(medium, medium)), m_medium(medium)
        {
            m_phaseFunction = medium->GetPhaseFunction();
            m_TNB = BuildTNB(-wo);
            m_InvTNB = glm::transpose(m_TNB);
        }
        ~MediumInteractionInfo() override;

        Vector3f ToLocalCoordinate(const Vector3f& v) const override;
        Vector3f ToWorldCoordinate(const Vector3f& v) const override;
        const Medium* GetMedium() const { return m_medium; }
        const Medium* GetMedium(const Vector3f wi) const override { return m_medium; }
        const PhaseFunction* GetPhaseFunction() const { return m_phaseFunction; }
        virtual bool IsSurfaceInteraction() const override { return false; }

    private:
        const Medium*           m_medium{};
        const PhaseFunction*    m_phaseFunction{};
        Matrix3f                m_TNB{}, m_InvTNB{};
    };

    class SurfaceInteraction : public InteractionInfo
    {
    public:
        SurfaceInteraction() {}
        ~SurfaceInteraction() override;

        // Spawn a ray from its current hit information
        RayTr SpawnRay(const Vector3f& dir) const override;
        RayTr SpawnRayTo(const Point3f& pos) const override;

        virtual const Medium* GetMedium(const Vector3f wi) const override;
        virtual SurfaceGeometryInfo GetGeometryInfo() const override;

        virtual bool IsSurfaceInteraction() const override { return true; }

        void SetHitInfo(float t, const Point3f& hitPos, const Vector3f& wo,
            const Vector3f& modelNormal, const Vector2f& uv, bool frontFace,
            const Vector3f& dpdu, const Vector3f& dpdv);

        bool IsFrontFace() const { return m_isFrontFace; }
        float GetDistance() const { return m_distance; }

        void SetHitPrimitive(const IRayPrimitive* primitive) { m_primitive = primitive; }
        const IRayPrimitive* GetHitPrimitive() const { return m_primitive; }

        Normal3f GetInteractionNormal() const { return m_isFrontFace ? m_modelNormal : -m_modelNormal; }
        Normal3f GetModelNormal() const { return m_modelNormal; }

        void SetTexCoord(const Point2f& texCoord) { m_texCoord = texCoord; }
        Point2f GetTexCoord() const { return m_texCoord; }

        void SetDpDu(const Vector3f& dir) { m_dpDu = dir; }
        Vector3f GetDpDu() const { return m_dpDu; }

        void SetDpDv(const Vector3f& dir) { m_dpDv = dir; }
        Vector3f GetDpDv() const { return m_dpDv; }

        void SetMediumInterface(const MediumInterface& mediumInterface) { m_mediumInterface = mediumInterface; }
        MediumInterface GetMediumInterface() const { return m_mediumInterface; }

        Matrix3f GetInteractionTNB() const;
        Matrix3f GetInteractionInverseTNB() const;

        Vector3f ToLocalCoordinate(const Vector3f& v) const override;
        Vector3f ToWorldCoordinate(const Vector3f& v) const override;

        /**
         * @brief Get the emitted radiance at a surface point intersected by a ray
         * @param w Direction of the ray in world coord
         * @return
        */
        Spectrum Le(const Vector3f& w) const;

        std::shared_ptr<BSDF> GetBSDF() const { return m_bsdf; }
        void SetBSDF(std::shared_ptr<BSDF> bsdf) { m_bsdf = bsdf; }

        const Material* GetMaterial() const
        {
            return m_primitive->GetMaterial();
        }

    private:
        float                   m_distance = std::numeric_limits<float>::infinity();
        bool                    m_isFrontFace = false;
        Vector3f                m_dpDu{},
                                m_dpDv{};
        Normal3f                m_modelNormal{};
        Vector2f                m_texCoord{};
        Matrix3f                m_TNB{};
        Matrix3f                m_invTNB{};

        std::shared_ptr<BSDF>   m_bsdf = nullptr;
        const IRayPrimitive*    m_primitive = nullptr;
        MediumInterface         m_mediumInterface{};

    };
}