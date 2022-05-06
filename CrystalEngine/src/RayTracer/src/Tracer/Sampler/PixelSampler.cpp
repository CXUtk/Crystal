#include "PixelSampler.h"

namespace tracer
{
    PixelSampler::PixelSampler(int samplesPerPixel, int dimensions)
        : Sampler(samplesPerPixel), m_dimensions(dimensions)
    {
        for (int i = 0; i < m_dimensions; i++)
        {
            m_samples1D.push_back(std::vector<float>(m_samplesPerPixel));
            m_samples2D.push_back(std::vector<Point2f>(m_samplesPerPixel));
        }
    }

    float PixelSampler::Get1D()
    {
        if (m_current1DDimension < m_samples1D.size())
        {
            return m_samples1D[m_current1DDimension++][m_currentSampleIndex];
        }
        return UniformRandomFloat(m_rng);
    }

    Point2f PixelSampler::Get2D()
    {
        if (m_current2DDimension < m_samples2D.size())
        {
            return m_samples2D[m_current2DDimension++][m_currentSampleIndex];
        }
        return Point2f(UniformRandomFloat(m_rng), UniformRandomFloat(m_rng));
    }

    bool PixelSampler::StartNextSample()
    {
        m_current1DDimension = 0, m_current2DDimension = 0;
        return Sampler::StartNextSample();
    }

    void PixelSampler::StartPixel(const Point2i& pt, int startSample, int endSample)
    {
        m_current1DDimension = 0, m_current2DDimension = 0;
        Sampler::StartPixel(pt, startSample, endSample);
    }
}
