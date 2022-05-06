#pragma once
#include "PixelSampler.h"

namespace tracer
{
    class StratifiedSampler : public PixelSampler
    {
    public:
        StratifiedSampler(Point2i pixelSamples, int sampledDimensions);
        ~StratifiedSampler() override;

        virtual std::shared_ptr<Sampler> Clone(int seed) const override;

        void StartPixel(const Point2i& pt, int startSample, int endSample = -1) override;

    private:
        Point2i m_pixelSamples{};
    };
}