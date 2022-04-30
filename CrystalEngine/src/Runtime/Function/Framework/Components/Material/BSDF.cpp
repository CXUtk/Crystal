#include "BSDF.h"
#include <glm/glm.hpp>
#include <Core/Interaction/SurfaceInteraction.h>

namespace crystal
{
    BSDF::BSDF(const SurfaceInteraction* si) : m_isec(si)
    {}

    void BSDF::AddBxDF(const std::shared_ptr<BxDF>& bxdf)
    {
        assert(m_numBxDF < MAX_BxDFs);
        m_bxdfs[m_numBxDF++] = bxdf;
    }

    BxDFType BSDF::Flags() const
    {
        BxDFType flag = BxDFType::None;
        for (int i = 0; i < m_numBxDF; i++)
        {
            flag = flag | m_bxdfs[i]->GetType();
        }
        return flag;
    }

    float BSDF::Pdf(const Vector3f& wOut, const Vector3f& wIn, BxDFType flags) const
    {
        auto transform = m_isec->GetInverseTNB();
        Vector3f wo = transform * wOut;
        Vector3f wi = transform * wIn;
        //fixVector(wo);
        //fixVector(wi);

        float pdf = 0.f;
        int matched = 0;
        bool reflect = wi.y * wo.y > 0;
        for (int i = 0; i < m_numBxDF; i++)
        {
            auto& bxdf = m_bxdfs[i];
            if (bxdf->Matches(flags) && (reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION))
                || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION)))
            {
                pdf += bxdf->Pdf(wo, wi);
                matched++;
            }
        }
        return matched > 0 ? pdf / matched : 0;
    }

    Spectrum BSDF::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        auto transform = m_isec->GetInverseTNB();
        Vector3f wo = transform * wOut;
        Vector3f wi = transform * wIn;
        //fixVector(wOut);
        //fixVector(wIn);
        Spectrum L(0);
        bool reflect = wi.y * wo.y > 0;
        for (int i = 0; i < m_numBxDF; i++)
        {
            auto& bxdf = m_bxdfs[i];
            if ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION))
                || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION)))
            {
                L += bxdf->DistributionFunction(wo, wi);
            }
        }
        return L;
    }

    Spectrum BSDF::SampleDirection(float sampleBSDF, const Vector2f& sample, const Vector3f& wOut, Vector3f* wIn,
            float* pdf, BxDFType flags, BxDFType* sampledType) const
    {
        // 最大10个
        assert(m_numBxDF < MAX_BxDFs);
        int tot = 0;
        int idMap[10] = { 0 };

        // 按照概率分布均匀采样
        for (int i = 0; i < m_numBxDF; i++)
        {
            auto& bxdf = m_bxdfs[i];
            if (bxdf->Matches(flags))
            {
                idMap[tot] = i;
                ++tot;
            }
        }

        if (!tot) return Spectrum(0.f);

        // Select one bxdf to sample wIn
        int v = (int)(std::min(0.9999999404f, sampleBSDF) * tot);
        std::shared_ptr<BxDF> selectedBxdf = m_bxdfs[idMap[v]];

        *sampledType = selectedBxdf->GetType();

        Vector3f wo = m_isec->GetInverseTNB() * wOut;
        // fixVector(wOut);

        auto L = selectedBxdf->SampleDirection(sample, wo, wIn, pdf, sampledType);
        if (L == Spectrum(0.f) || *pdf == 0.f) return Spectrum(0.f);

        if (!selectedBxdf->Contains(BxDFType::BxDF_SPECULAR))
        {
            for (int i = 0; i < m_numBxDF; i++)
            {
                auto& bxdf = m_bxdfs[i];
                if (bxdf != selectedBxdf && bxdf->Matches(flags))
                {
                    *pdf += bxdf->Pdf(wo, *wIn);
                }
            }

            bool reflect = wIn->y * wo.y > 0;
            for (int i = 0; i < m_numBxDF; i++)
            {
                auto& bxdf = m_bxdfs[i];
                if (bxdf != selectedBxdf && bxdf->Matches(flags) &&
                    ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION))
                        || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION))))
                {
                    L += bxdf->DistributionFunction(wo, *wIn);
                }
            }
        }
        *wIn = m_isec->GetTNB() * (*wIn);
        // fixVector(*wIn);

        *pdf /= tot;
        return L;
    }
}