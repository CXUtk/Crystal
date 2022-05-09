#pragma once
#include <Core/Math/Math.h>
#include <Core/Math/Geometry.h>
#include <Core/Sampling/DistributionSampler.h>

#include <string>
#include <vector>
#include <stbi/stb_image.h>
#include <algorithm>

#include "CPUTexture.h"


namespace crystal
{
    enum class DistributionMapType
    {
        Regular,
        ThetaPhi
    };

    class CPUTexture2D
    {
    public:
        virtual ~CPUTexture2D() = 0 {};
        virtual Vector3f Sample(const Vector2f& uv) const = 0;
        virtual Vector3f SampleMipmap(const Vector2f& uv, float lod) const = 0;

        virtual std::shared_ptr<Distribution2D> GetDistributionSampler(DistributionMapType type) const = 0;
        //virtual Vector2f WeightedSampleUV(const Vector2f& sample) const = 0;
        //virtual Float AverageWeights() const = 0;
        //virtual Float Pdf(const Vector2f& u) const = 0;
    private:

    };

    class CPUTexture2DPure : public CPUTexture2D
    {
    public:
        CPUTexture2DPure(const Spectrum& color) : m_color(color) {}
        ~CPUTexture2DPure() override {}

        virtual Vector3f Sample(const Vector2f& uv) const override { return m_color; }
        virtual Vector3f SampleMipmap(const Vector2f& uv, float lod) const override { return m_color; }
        virtual std::shared_ptr<Distribution2D> GetDistributionSampler(DistributionMapType type) const override;
        //// Int_{0}^{\pi} sin(x) dx
        //virtual Float AverageWeights() const override
        //{
        //    return (m_color.r + m_color.g + m_color.b) * 2 / (3.f * glm::pi<float>());
        //}
        //virtual Vector2f WeightedSampleUV(const Vector2f& sample) const
        //{
        //    float v = std::acos(-2 * sample.y + 1) / glm::pi<float>();
        //    return Vector2f(sample.x, v);
        //}
        //virtual Float Pdf(const Vector2f& u) const override
        //{
        //    return  std::sin(u.y * glm::pi<float>()) * glm::pi<float>() / 2.f;
        //}

    private:
        Spectrum m_color{};
    };

    class CPUTexture2DCheckerBoard : public CPUTexture2D
    {
    public:
        CPUTexture2DCheckerBoard(const Spectrum& W, const Spectrum& B) : m_W(W), m_B(B) {}
        ~CPUTexture2DCheckerBoard() override {}

        virtual Vector3f Sample(const Vector2f& uv) const override;
        virtual Vector3f SampleMipmap(const Vector2f& uv, float lod) const override { return Sample(uv); }
        virtual std::shared_ptr<Distribution2D> GetDistributionSampler(DistributionMapType type) const override;

        //virtual Vector2f WeightedSampleUV(const Vector2f& sample) const override;
        //virtual Float AverageWeights() const override
        //{
        //    Spectrum combine = m_W + m_B;
        //    return (combine.r + combine.g + combine.b) / 3.f;
        //}
        //virtual Float Pdf(const Vector2f& u) const override;
    private:
        Spectrum m_W{}, m_B{};
    };


    class CPUImageTexture2D : public CPUTexture2D
    {
    public:
        CPUImageTexture2D(int width, int height, bool isHDR = false);
        CPUImageTexture2D(int width, int height, float* data);
        CPUImageTexture2D(int width, int height, stbi_uc* data, bool isHDR = false);

        ~CPUImageTexture2D() override;

        Vector3f Sample(const Vector2f& uv) const override;
        virtual Vector3f SampleMipmap(const Vector2f& uv, float lod) const override;

        Vector3f Sample(const Vector2i& coord) const;

        void SetPixel(const Vector2i& coord, const Vector3f& value);
        void SetPixel(const Vector2f& uv, const Vector3f& value);

        std::vector<stbi_uc> GetByteData() const;
        std::vector<float> GetFloatData(int components = 3) const;

        virtual std::shared_ptr<Distribution2D> GetDistributionSampler(DistributionMapType type) const override;
    private:
        size_t      m_width = 0, m_height = 0;
        bool        m_isHDR = false;
        Vector3f*   m_data = nullptr;

        void SetUpSampler();
    };
}