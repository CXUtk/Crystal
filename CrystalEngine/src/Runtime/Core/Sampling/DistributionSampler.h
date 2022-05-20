#pragma once
#include <Core/Math/Math.h>
#include <Core/Math/Geometry.h>
#include "Sampling.h"

#include <string>
#include <vector>
#include <stbi/stb_image.h>
#include <algorithm>

namespace crystal
{
    class Distribution1D
    {
    public:
        virtual ~Distribution1D() = 0 {};
        virtual Float SampleContinuous(Float sample, Float* pdf) = 0;
        virtual Float ContinuousPdf(Float x) = 0;
        virtual Float FuncInt() const = 0;
    };

    class Distribution2D
    {
    public:
        virtual ~Distribution2D() = 0 {};
        virtual Vector2f SampleContinuous(const Vector2f& sample, Float* pdf) = 0;
        virtual Float ContinuousPdf(Vector2f uv) = 0;
        virtual Float FuncInt() const = 0;
    };

    class Distribution1DDiscrete : public Distribution1D
    {
    public:
        Distribution1DDiscrete(const Float* values, size_t N);
        ~Distribution1DDiscrete() override;

        Float DiscretePdf(size_t index) const;
        size_t SampleDiscrete(Float sample, Float* pdf, Float* remapped) const;
        Float SampleContinuous(Float sample, Float* pdf) override;
        Float AvgWeright() const { return m_funcInt; }
        size_t GetColIndex(Float x) { return std::clamp((size_t)std::floor(m_N * x),
            (size_t)0, (size_t)(m_N - 1)); }
        Float ContinuousPdf(Float x) override;
        Float FuncInt() const override { return m_funcInt; }

    private:
        std::vector<Float>  m_cdf{};
        Float               m_funcInt{};
        size_t              m_N{};
    };

    class Distribution2DDiscrete : public Distribution2D
    {
    public:
        Distribution2DDiscrete(const Float* values, size_t N, size_t M);
        ~Distribution2DDiscrete() override;
        Vector2f SampleContinuous(const Vector2f& sample, Float* pdf) override;
        Float ContinuousPdf(Vector2f uv) override;
        Float FuncInt() const override { return m_single->AvgWeright(); }

    private:
        std::vector<Distribution1DDiscrete>     m_rows{};
        std::unique_ptr<Distribution1DDiscrete> m_single{};
    };

    class Distribution2DPureSineTheta : public Distribution2D
    {
    public:
        Distribution2DPureSineTheta();
        ~Distribution2DPureSineTheta() override;
        Vector2f SampleContinuous(const Vector2f& sample, Float* pdf) override;
        Float ContinuousPdf(Vector2f uv) override;
        Float FuncInt() const override { return 2.f / glm::pi<float>(); }

    private:
    };
}