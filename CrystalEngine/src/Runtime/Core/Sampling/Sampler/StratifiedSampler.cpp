#include "StratifiedSampler.h"

namespace crystal
{
    StratifiedSampler::StratifiedSampler(Point2i pixelSamples, int sampledDimensions)
        : PixelSampler(pixelSamples.x * pixelSamples.y, sampledDimensions), m_pixelSamples(pixelSamples)

    {}

    /**
     * @brief Generate stratified samples for a fixed-size array
     * @param A 
     * @param size 
     * @param rng 
    */
    void StratifiedSample1D(float* A, int size, RNG& rng)
    {
        float invNSamples = 1.f / size;
        for (int i = 0; i < size; i++)
        {
            A[i] = std::min(OneMinusEpsilon, invNSamples * (i + UniformRandomFloat(rng)));
        }
    }

    /**
     * @brief Generate stratified 2D samples for a fixed-size array
     * @param A 
     * @param size 
     * @param rng 
    */
    void StratifiedSample2D(Point2f* A, const Point2i& size, RNG& rng)
    {
        float invNSamplesX = 1.f / size.x;
        float invNSamplesY = 1.f / size.y;
        for (int i = 0; i < size.y; i++)
        {
            for (int j = 0; j < size.x; j++)
            {
                A->x = std::min(OneMinusEpsilon, invNSamplesX * (j + UniformRandomFloat(rng)));
                A->y = std::min(OneMinusEpsilon, invNSamplesY * (i + UniformRandomFloat(rng)));
                A++;
            }
        }
    }

    /**
     * @brief Generate a well-distributed set of k-dimension samples for a fixed-size array
     * @param A 
     * @param size 
     * @param dim 
     * @param rng 
    */
    void LatinHypercube(float* A, int size, int dim, RNG& rng)
    {
        float invNSamples = 1.f / size;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                A[i * dim + j] = std::min(OneMinusEpsilon, invNSamples * (i + UniformRandomFloat(rng)));
            }
        }

        for (int j = 0; j < dim; j++)
        {
            for (int i = size - 1; i > 0; i--)
            {
                int target = rng() % (i + 1);
                std::swap(A[i * dim + j], A[target * dim + j]);
            }
        }
    }

    StratifiedSampler::~StratifiedSampler()
    {}

    std::shared_ptr<Sampler> StratifiedSampler::Clone(int seed) const
    {
        auto other = std::make_shared<StratifiedSampler>(*this);
        other->m_rng.seed(seed);
        return other;
    }

    void StratifiedSampler::StartPixel(const Point2i& pt, int startSample, int endSample)
    {
        PixelSampler::StartPixel(pt, startSample, endSample);
        for (int i = 0; i < m_samples1D.size(); i++)
        {
            StratifiedSample1D(&m_samples1D[i][0], m_pixelSamples.x * m_pixelSamples.y, m_rng);
            std::shuffle(m_samples1D[i].begin(), m_samples1D[i].begin() + m_pixelSamples.x * m_pixelSamples.y, m_rng);
        }
        for (int i = 0; i < m_samples2D.size(); i++)
        {
            StratifiedSample2D(&m_samples2D[i][0], m_pixelSamples, m_rng);
            std::shuffle(m_samples2D[i].begin(), m_samples2D[i].begin() + m_pixelSamples.x * m_pixelSamples.y, m_rng);
        }

        for (int i = 0; i < m_samples1DArraySizes.size(); i++)
        {
            for (int j = 0; j < m_samplesPerPixel; j++)
            {
                int count = m_samples1DArraySizes[i];
                StratifiedSample1D(&m_sampleArray1D[i][j * count], count, m_rng);
                std::shuffle(m_sampleArray1D[i].begin() + j * count,
                    m_sampleArray1D[i].begin() + j * count + count, m_rng);
            }
        }

        for (int i = 0; i < m_samples2DArraySizes.size(); i++)
        {
            for (int j = 0; j < m_samplesPerPixel; j++)
            {
                int count = m_samples2DArraySizes[i];
                LatinHypercube((float*)&m_sampleArray2D[i][j * count], count, 2, m_rng);
            }
        }
    }
}