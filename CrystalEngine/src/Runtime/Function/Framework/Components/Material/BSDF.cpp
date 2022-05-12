#include "BSDF.h"
#include <glm/glm.hpp>
#include <Core/Interaction/SurfaceInteraction.h>
#include <Core/Sampling/Sampling.h>

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
        float pdf = 0.f;
        int matched = 0;
        bool reflect = wIn.y * wOut.y > 0;
        for (int i = 0; i < m_numBxDF; i++)
        {
            auto& bxdf = m_bxdfs[i];
            if (bxdf->Matches(flags) && (reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION))
                || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION)))
            {
                pdf += bxdf->Pdf(wOut, wIn);
                matched++;
            }
        }
        return matched > 0 ? pdf / matched : 0;
    }

    Spectrum BSDF::DistributionFunction(const Vector3f& wOut, const Vector3f& wIn) const
    {
        Spectrum L(0);
        bool reflect = wIn.y * wOut.y > 0;
        for (int i = 0; i < m_numBxDF; i++)
        {
            auto& bxdf = m_bxdfs[i];
            if ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION))
                || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION)))
            {
                L += bxdf->DistributionFunction(wOut, wIn);
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

        auto L = selectedBxdf->SampleDirection(sample, wOut, wIn, pdf, sampledType);
        if (L == Spectrum(0.f) || *pdf == 0.f) return Spectrum(0.f);

        if (!selectedBxdf->Contains(BxDFType::BxDF_SPECULAR))
        {
            for (int i = 0; i < m_numBxDF; i++)
            {
                auto& bxdf = m_bxdfs[i];
                if (bxdf != selectedBxdf && bxdf->Matches(flags))
                {
                    *pdf += bxdf->Pdf(wOut, *wIn);
                }
            }

            bool reflect = wIn->y * wOut.y >= 0;
            for (int i = 0; i < m_numBxDF; i++)
            {
                auto& bxdf = m_bxdfs[i];
                if (bxdf != selectedBxdf && bxdf->Matches(flags) &&
                    ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION))
                        || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION))))
                {
                    L += bxdf->DistributionFunction(wOut, *wIn);
                }
            }
        }

        *pdf /= tot;
        return L;
    }


    Spectrum BSDF::CalculateBSDFNoLDivideByPdf(const Vector3f& wOut, const Vector3f& wIn,
        BxDFType flags) const
    {
        Float pdf;
        // 最大8个
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

        if (tot == 1)
        {
            return m_bxdfs[idMap[0]]->CalculateBSDFNoLDivideByPdf(wOut, wIn, flags);
        }
        else
        {
            throw std::runtime_error("Not implemented");
        }

        //if (!tot) return Spectrum(0.f);

        //// Select one bxdf to sample wIn
        //int v = (int)(std::min(0.9999999404f, sampleBSDF) * tot);
        //std::shared_ptr<BxDF> selectedBxdf = m_bxdfs[idMap[v]];

        //*sampledType = selectedBxdf->GetType();

        //Vector3f wo = m_isec->GetInverseTNB() * wOut;
        //FixVector(wo);

        //auto L = selectedBxdf->SampleDirection(sample, wo, wIn, pdf, sampledType);
        //if (L == Spectrum(0.f) || *pdf == 0.f) return Spectrum(0.f);

        //if (!selectedBxdf->Contains(BxDFType::BxDF_SPECULAR))
        //{
        //    for (int i = 0; i < m_numBxDF; i++)
        //    {
        //        auto& bxdf = m_bxdfs[i];
        //        if (bxdf != selectedBxdf && bxdf->Matches(flags))
        //        {
        //            *pdf += bxdf->Pdf(wo, *wIn);
        //        }
        //    }

        //    bool reflect = wIn->y * wo.y >= 0;
        //    for (int i = 0; i < m_numBxDF; i++)
        //    {
        //        auto& bxdf = m_bxdfs[i];
        //        if (bxdf != selectedBxdf && bxdf->Matches(flags) &&
        //            ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION))
        //                || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION))))
        //        {
        //            L += bxdf->DistributionFunction(wo, *wIn);
        //        }
        //    }
        //}
        //*wIn = glm::normalize(m_isec->GetTNB() * (*wIn));
        //FixVector(*wIn);

        //*pdf /= tot;
        //return L;
        //return Spectrum();
    }
    Spectrum BxDF::CalculateBSDFNoLDivideByPdf(const Vector3f& wOut, const Vector3f& wIn, BxDFType scatterType) const
    {
        bool transmission = scatterType & BxDFType::BxDF_TRANSMISSION;
        Float NdotL = std::max(0.f, transmission ? -wIn.y : wIn.y);
        return DistributionFunction(wOut, wIn) * NdotL / Pdf(wOut, wIn);
    }

}