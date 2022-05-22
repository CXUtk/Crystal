#include "Sampler.h"
#include <Core/Sampling/Sampling.h>
using namespace crystal;

namespace crystal
{

    //std::shared_ptr<Sampler> Sampler::LoadSampler(JsonNode_CPTR pSamplerNode, const config::ConfigInfo& configInfo)
    //{
    //    assert(pSamplerNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);

    //    auto samplerType = pSamplerNode->GetMember("Type")->GetString();
    //    auto samplerSeed = pSamplerNode->GetMember("Seed")->GetInt();

    //    if (samplerType == "Default")
    //    {
    //        return std::make_shared<DefaultSampler>(configInfo.SamplesPerPixel, samplerSeed);
    //    }
    //    else if (samplerType == "Stratified")
    //    {
    //        bool isSQR = false;
    //        int side = 0;
    //        for (int i = 1; i <= configInfo.SamplesPerPixel; i++)
    //        {
    //            if ((long long)i * i > configInfo.SamplesPerPixel)
    //            {
    //                break;
    //            }
    //            if (i * i == configInfo.SamplesPerPixel)
    //            {
    //                side = i;
    //                isSQR = true;
    //            }
    //        }
    //        if (!isSQR)
    //        {
    //            throw std::exception("Stratified sampler should have a square number of SamplesPerPixel");
    //        }
    //        return std::make_shared<StratifiedSampler>(Point2i(side, side), 18);
    //    }
    //    else if (samplerType == "Halton")
    //    {
    //        return std::make_shared<HaltonSampler>(configInfo.SamplesPerPixel, 
    //            Bound2i(Point2i(0, 0), Point2i(configInfo.Width, configInfo.Height)));
    //    }
    //    else
    //    {
    //        throw std::invalid_argument("Invalid Sampler Type!");
    //    }
    //    return nullptr;
    //}

    void Sampler::Request1DArray(int N)
    {
        m_samples1DArraySizes.push_back(N);
        m_sampleArray1D.push_back(std::vector<float>(N * m_samplesPerPixel));
    }

    void Sampler::Request2DArray(int N)
    {
        m_samples2DArraySizes.push_back(N);
        m_sampleArray2D.push_back(std::vector<Point2f>(N * m_samplesPerPixel));
    }

    float* Sampler::Get1DArray(int N)
    {
        if (m_array1DOffset == m_sampleArray1D.size())
        {
            return nullptr;
        }
        return &m_sampleArray1D[m_array1DOffset++][m_currentSampleIndex * N];
    }

    Point2f* Sampler::Get2DArray(int N)
    {
        if (m_array2DOffset == m_sampleArray2D.size())
        {
            return nullptr;
        }
        return &m_sampleArray2D[m_array2DOffset++][m_currentSampleIndex * N];
    }

    bool Sampler::StartNextSample()
    {
        m_array1DOffset = m_array2DOffset = 0;
        return ++m_currentSampleIndex < m_endSampleIndex;
    }

    void Sampler::StartPixel(const Point2i& pt, int startSample, int endSample)
    {
        m_currentPixel = pt; 
        m_startSampleIndex = startSample;
        m_endSampleIndex = endSample;
        m_currentSampleIndex = m_startSampleIndex;
        if (m_endSampleIndex < 0)
        {
            m_endSampleIndex = m_samplesPerPixel;
        }
        m_array1DOffset = m_array2DOffset = 0;
    }

}