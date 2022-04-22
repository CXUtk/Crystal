#pragma once
#include <Core/Math/Math.h>
#include <Core/Math/Geometry.h>

#include <string>
#include <vector>
#include <stbi/stb_image.h>
#include <algorithm>

namespace crystal
{
    struct CubeUV
    {
        int         Id;
        Vector2f    UV;
    };

    CubeUV XYZToCubeUV(const Vector3f& p);
    Vector3f CubeUVToVector(const CubeUV& cube);
    Vector3f sRGBToHDR(const Vector3f& color);
    Vector3f HDRTosRGB(const Vector3f& hdr);

    class RawTexture2D
    {
    public:
        RawTexture2D(int width, int height, bool isHDR = false)
            : Width(width), Height(height), IsHDR(isHDR)
        {
            Data = new Vector3f[Width * Height];
            memset(Data, 0, Width * Height * sizeof(Vector3f));
            SetUpSampler();
        }

        RawTexture2D(int width, int height, float* data)
            : Width(width), Height(height)
        {
            Data = new Vector3f[Width * Height];
            memcpy(Data, data, Width * Height * sizeof(Vector3f));
            SetUpSampler();
        }

        RawTexture2D(int width, int height, stbi_uc* data, bool isHDR = false)
            : Width(width), Height(height), IsHDR(isHDR)
        {
            Data = new Vector3f[Width * Height];

            int channels = 3;
            for (int i = 0; i < Width * Height; i++)
            {
                size_t row = i / Width;
                size_t col = i % Width;
                size_t srcId = (row * Width + col) * 3;
                Data[i].r = data[srcId] / 255.f;
                Data[i].g = data[srcId + 1] / 255.f;
                Data[i].b = data[srcId + 2] / 255.f;

                if (IsHDR)
                {
                    Data[i] = sRGBToHDR(Data[i]);
                }
            }
            SetUpSampler();
        }

        ~RawTexture2D()
        {
            delete[] Data;
            Data = nullptr;
            delete[] WeightForRow;
            WeightForRow = nullptr;
            delete[] WeightForCol;
            WeightForCol = nullptr;
        }

        Vector3f Sample(Vector2f coord) const
        {
            size_t row = (size_t)(std::min(1.0 - coord.y, 0.9999) * Height);
            size_t col = (size_t)(std::min(coord.x, 0.9999f) * Width);
            return Data[row * Width + col];
        }

        Vector3f Sample(Vector2i coord) const
        {
            assert(coord.x >= 0 && coord.x < Width&& coord.y >= 0 && coord.y < Height);
            size_t row = Height - coord.y - 1;
            return Data[row * Width + coord.x];
        }

        void SetPixel(Vector2i coord, Vector3f value)
        {
            assert(coord.x >= 0 && coord.x < Width&& coord.y >= 0 && coord.y < Height);
            size_t row = Height - coord.y - 1;
            Data[row * Width + coord.x] = value;
        }

        void SetPixel(Vector2f coord, Vector3f value)
        {
            size_t row = (size_t)(std::min(1.0 - coord.y, 0.9999) * Height);
            size_t col = (size_t)(std::min(coord.x, 0.9999f) * Width);
            Data[row * Width + col] = value;
        }

        std::vector<stbi_uc> GetByteData() const
        {
            std::vector<stbi_uc> data;
            data.reserve(Width * Height * 3);

            for (int i = 0; i < Width * Height; i++)
            {
                Vector3f v = glm::clamp(Data[i], Vector3f(0.f), Vector3f(1.f));
                if (IsHDR)
                {
                    Data[i] = HDRTosRGB(Data[i]);
                }
                data.push_back((unsigned char)floor(v.r * 255));
                data.push_back((unsigned char)floor(v.g * 255));
                data.push_back((unsigned char)floor(v.b * 255));
            }
            return data;
        }

        std::vector<float> GetFloatData(int components = 3) const
        {
            std::vector<float> data;
            data.resize(Width * Height * components);

            for (int i = 0; i < Width * Height; i++)
            {
                auto& v = Data[i];
                for (int j = 0; j < components; j++)
                {
                    data[i * components + j] = v[j];
                }
            }
            return data;
        }

        void MipmapCompress(int compressLevel)
        {
            size_t newWidth = Width >> compressLevel;
            size_t newHeight = Height >> compressLevel;
            size_t convSize = 1ull << compressLevel;

            Vector3f* newData = new Vector3f[newWidth * newHeight];

            for (size_t i = 0; i < newHeight; i++)
            {
                for (size_t j = 0; j < newWidth; j++)
                {
                    size_t row_s = i << compressLevel;
                    size_t col_s = j << compressLevel;

                    Vector3f sumValue(0.f);
                    for (size_t r = row_s; r < row_s + convSize; r++)
                    {
                        for (size_t c = col_s; c < col_s + convSize; c++)
                        {
                            sumValue += Data[r * Width + c];
                        }
                    }
                    newData[i * newWidth + j] = sumValue * (1.f / (convSize * convSize));
                }
            }
            delete[] Data;
            delete[] WeightForRow;
            delete[] WeightForCol;
            Data = newData;

            Width = newWidth;
            Height = newHeight;
            SetUpSampler();
        }

        void SetUpSampler()
        {
            WeightForRow = new float[Height + 1];
            memset(WeightForRow, 0, sizeof(float) * (Height + 1));
            WeightForCol = new float[(Width + 1) * Height];
            memset(WeightForCol, 0, sizeof(float) * ((Width + 1) * Height));

            for (size_t i = 0; i < Height; i++)
            {
                for (size_t j = 0; j < Width; j++)
                {
                    auto data = Data[i * Width + j];
                    float w = data.x * 0.3f + data.y * 0.6f + data.z * 0.1f;
                    TotalWeight += w;
                    WeightForRow[i + 1] += w;
                    WeightForCol[i * (Width + 1) + j + 1] = w;
                }
                for (size_t j = 1; j <= Width; j++)
                {
                    WeightForCol[i * (Width + 1) + j] += WeightForCol[i * (Width + 1) + j - 1];
                }
            }

            for (size_t i = 1; i <= Height; i++)
            {
                WeightForRow[i] += WeightForRow[i - 1];
            }
        }

        Vector2f WeightedSampleUV(const Vector2f& sample)
        {
            int row = std::upper_bound(WeightForRow, WeightForRow + Height + 1,
                sample.x * WeightForRow[Height]) - WeightForRow - 1;

            float* start = WeightForCol + row * (Width + 1);
            int col = std::upper_bound(start, start + Width + 1,
                sample.y * start[Width]) - start - 1;
            return Vector2f((float)col / Width, (float)(Height - row - 1) / Height);
        }

        size_t Width = 0, Height = 0;
        bool IsHDR = false;
        Vector3f* Data = nullptr;

        float TotalWeight{};
        float* WeightForRow = nullptr;
        float* WeightForCol = nullptr;
    };
}