#include "DistributionSampler.h"

namespace crystal
{
    Distribution1DDiscrete::Distribution1DDiscrete(const Float* values, size_t N)
        : m_N(N)
    {
        m_cdf.resize(N + 1, 0.f);
        m_cdf[0] = 0;
        for (size_t i = 1; i <= N; i++)
        {
            m_cdf[i] = m_cdf[i - 1] + values[i - 1] / N;
        }

        // Transform to cdf format [0, 1], funcInt = (\sum a_i) / N
        // pdf(i) = a_i / funcInt
        m_funcInt = m_cdf[N];
        if (m_funcInt == 0.f)
        {
            for (size_t i = 1; i <= N; i++)
            {
                m_cdf[i] = ((Float)i / N);
            }
        }
        else
        {
            for (size_t i = 1; i <= N; i++)
            {
                m_cdf[i] /= m_funcInt;
            }
        }
    }

    Distribution1DDiscrete::~Distribution1DDiscrete()
    {}

    Float Distribution1DDiscrete::DiscretePdf(size_t index) const
    {
        return m_cdf[index + 1] - m_cdf[index];
    }

    size_t Distribution1DDiscrete::SampleDiscrete(Float sample, Float* pdf, Float* remapped) const
    {
        size_t index = std::upper_bound(m_cdf.begin(), m_cdf.end(), sample) - m_cdf.begin() - 1;
        index = std::max(0ull, std::min(index, m_N - 1ull));
        *pdf = DiscretePdf(index);
        *remapped = (sample - m_cdf[index]) / (m_cdf[index + 1] - m_cdf[index]);
        return index;
    }

    Float Distribution1DDiscrete::SampleContinuous(Float sample, Float* pdf)
    {
        size_t index = std::upper_bound(m_cdf.begin(), m_cdf.end(), sample) - m_cdf.begin() - 1;
        index = std::max(0ull, std::min(index, m_N - 1ull));

        Float du = (sample - m_cdf[index]) / (m_cdf[index + 1] - m_cdf[index]);
        *pdf = (m_cdf[index + 1] - m_cdf[index]) * m_N;
        return (index + du) / m_N;
    }

    Float Distribution1DDiscrete::ContinuousPdf(Float x)
    {
        size_t i = GetColIndex(x);
        return (m_cdf[i + 1] - m_cdf[i]) * m_N;
    }

    Distribution2DDiscrete::Distribution2DDiscrete(const Float* values, size_t N, size_t M)
    {
        m_rows.reserve(N);

        std::vector<Float> marginalFunc;
        marginalFunc.reserve(N);

        for (int i = 0; i < N; i++)
        {
            m_rows.push_back(Distribution1DDiscrete(&values[i * M], M));
            marginalFunc.push_back(m_rows.back().AvgWeright());
        }

        m_single = std::make_unique<Distribution1DDiscrete>(marginalFunc.data(), N);
    }

    Distribution2DDiscrete::~Distribution2DDiscrete()
    {}

    Vector2f Distribution2DDiscrete::SampleContinuous(const Vector2f& sample, Float* pdf)
    {
        Float p1, p2;
        Vector2f res{};
        res.y = m_single->SampleContinuous(sample.y, &p1);

        size_t row = m_single->GetColIndex(res.y);
        res.x = m_rows[row].SampleContinuous(sample.x, &p2);
        *pdf = p1 * p2;
        return res;
    }

    Float Distribution2DDiscrete::ContinuousPdf(Vector2f uv)
    {
        size_t row = m_single->GetColIndex(uv.y);
        return m_single->ContinuousPdf(uv.y) * m_rows[row].ContinuousPdf(uv.x);
    }

    //size_t Distribution1D::SampleDiscrete(Float sample, Float* pdf, Float* remapped) const
    //{
    //    size_t index = std::upper_bound(m_cdf.begin(), m_cdf.end(), sample) - m_cdf.begin() - 1;
    //    *remapped = (sample - m_cdf[index]) / (m_cdf[index + 1] - m_cdf[index]);
    //    *pdf = Pdf(index);
    //    return index;
    //}

    Distribution2DPureSineTheta::Distribution2DPureSineTheta()
    {}

    Distribution2DPureSineTheta::~Distribution2DPureSineTheta()
    {}

    Vector2f Distribution2DPureSineTheta::SampleContinuous(const Vector2f& sample, Float* pdf)
    {
        float v = std::acos(-2 * sample.y + 1) / glm::pi<float>();
        *pdf = std::sin(v * glm::pi<float>()) * glm::pi<float>() / 2.f;
        return Vector2f(sample.x, v);
    }

    Float Distribution2DPureSineTheta::ContinuousPdf(Vector2f uv)
    {
        return std::sin(uv.y * glm::pi<float>()) * glm::pi<float>() / 2.f;
    }
}
