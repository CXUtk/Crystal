#pragma once
#include <Core/Math/Math.h>
#include <Core/Math/Geometry.h>

#include <string>
#include <vector>
#include <stbi/stb_image.h>
#include <algorithm>

#include "CPUTexture.h"

namespace crystal
{
    class CPUTexture2D
    {
    public:
        virtual ~CPUTexture2D() = 0 {};
        virtual Vector3f Sample(const Vector2f& uv) const = 0;
        virtual Vector3f SampleMipmap(const Vector2f& uv, float lod) const = 0;
    };

    class CPUTexture2DPure : public CPUTexture2D
    {
    public:
        CPUTexture2DPure(const Spectrum& color) : m_color(color) {}
        ~CPUTexture2DPure() override {}

        virtual Vector3f Sample(const Vector2f& uv) const override { return m_color; }
        virtual Vector3f SampleMipmap(const Vector2f& uv, float lod) const override { return m_color; }

    private:
        Spectrum m_color;
    };

    class CPUTexture2DCheckerBoard : public CPUTexture2D
    {
    public:
        CPUTexture2DCheckerBoard(const Spectrum& W, const Spectrum& B) : m_W(W), m_B(B) {}
        ~CPUTexture2DCheckerBoard() override {}

        virtual Vector3f Sample(const Vector2f& uv) const override;
        virtual Vector3f SampleMipmap(const Vector2f& uv, float lod) const override { return Sample(uv); }

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

    private:
        size_t      m_width = 0, m_height = 0;
        bool        m_isHDR = false;
        Vector3f*   m_data = nullptr;

        float       TotalWeight{};
        float*      WeightForRow = nullptr;
        float*      WeightForCol = nullptr;
    };
}