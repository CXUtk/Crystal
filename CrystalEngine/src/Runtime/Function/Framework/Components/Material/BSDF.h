#pragma once
#include <Crystal.h>
#include <memory>
#include <vector>
#include <Core/Math/Geometry.h>
#include <Platform/RHI/Graphics/GraphicsCommon.h>

namespace crystal
{
    BIT_FLAGS_BEGIN(BxDFType, uint32_t)
    {
        None = 0,
        BxDF_REFLECTION = 1 << 0,
        BxDF_TRANSMISSION = 1 << 1,
        BxDF_GLOSSY = 1 << 2,
        BxDF_SPECULAR = 1 << 3,
        BxDF_DIFFUSE = 1 << 4,
        BxDF_ALL = 0x1F,
    };
    BIT_FLAGS_END(BxDFType);

    class SurfaceInteraction;
    class BxDF;

    class BSDF
    {
    public:
        BSDF(const SurfaceInteraction* si);
        int NumBxDFs() const { return m_numBxDF; }
        void AddBxDF(const std::shared_ptr<BxDF>& bxdf);
        BxDFType Flags() const;

        bool IsEmpty() const { return m_numBxDF == 0; }

        float Pdf(const Vector3f& wOut, const Vector3f& wIn, BxDFType flags) const;
        Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const;
        Spectrum SampleDirection(float sampleBSDF, const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn,
            float* pdf, BxDFType flags, BxDFType* sampledType) const;

        Spectrum CalculateBSDFNoLDivideByPdf(const Vector3f& wOut, const Vector3f& wIn, BxDFType flags) const;

    private:
        static constexpr int MAX_BxDFs = 8;

        const SurfaceInteraction*   m_isec = nullptr;
        std::shared_ptr<BxDF>       m_bxdfs[MAX_BxDFs];
        int                         m_numBxDF = 0;
    };

    class BxDF
    {
    public:
        BxDF(BxDFType type) : m_bxdfType(type) {}
        virtual ~BxDF() = 0 {}

        /**
         * @brief Each wOut and wIn are normalized to TNB coordinates
        */
        virtual Spectrum DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const = 0;

        virtual float Pdf(const Vector3f& wOut, const Vector3f& wIn) const = 0;

        /**
         * @brief Each wOut and wIn are normalized to TNB coordinates
         * @param sample
         * @param wOut
         * @param wIn
         * @param pdf
         * @param sampledType
         * @return
        */
        virtual Spectrum SampleDirection(const Vector2f& sample, const Vector3f& wOut,
            Vector3f* wIn, float* pdf, BxDFType* sampledType) const = 0;

        virtual Spectrum CalculateBSDFNoLDivideByPdf(const Vector3f& wOut, const Vector3f& wIn, BxDFType scatterType) const;

        bool Matches(BxDFType type) const { return !((~type) & m_bxdfType); }
        bool Contains(BxDFType type) const { return (m_bxdfType & type); }

        BxDFType GetType() const { return m_bxdfType; }

    private:
        BxDFType m_bxdfType{};
    };
}