#pragma once
#include "Sampler.h"
#include <Core/Sampling/Sampling.h>

namespace crystal
{
    class PixelSampler : public Sampler
    {
    public:
        PixelSampler(int samplesPerPixel, int dimensions);

        float Get1D() override;
        crystal::Point2f Get2D() override;
        bool StartNextSample() override;
        void StartPixel(const Point2i& pt, int startSample, int endSample = -1) override;

        virtual std::shared_ptr<Sampler> Clone(int seed) const = 0;

    protected:
        // Each dimension is stratified according to samplesPerPixel
        std::vector<std::vector<float>>             m_samples1D{};
        std::vector<std::vector<crystal::Point2f>>  m_samples2D{};

        int m_dimensions{};
        int m_current1DDimension = 0,
            m_current2DDimension = 0;
        RNG m_rng{};
    };
}