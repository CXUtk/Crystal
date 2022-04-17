#pragma once
#include <Core/Math/Math.h>
#include <Core/Math/Geometry.h>

#include <string>
#include <vector>
#include <stbi/stb_image.h>

namespace crystal
{
    inline Vector3f sRGBToHDR(const Vector3f& color)
    {
        return glm::pow(color, Vector3f(2.2f));
    }

    inline Vector3f HDRTosRGB(const Vector3f& hdr)
    {
        return glm::pow(hdr, Vector3f(1 / 2.2f));
    }

    struct CubeUV
    {
        int         Id;
        Vector2f    UV;
    };

    class RawTexture2D
    {
    public:
        RawTexture2D(int width, int height, bool isHDR = false)
            : Width(width), Height(height), IsHDR(isHDR)
        {
            Data = new Vector3f[Width * Height];
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
        }

        ~RawTexture2D()
        {
            delete[] Data;
        }

        Vector3f Sample(Vector2f coord) const
        {
            size_t row = (size_t)(std::min(1.0 - coord.y, 0.9999) * Height);
            size_t col = (size_t)(coord.x * Width);
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

        std::vector<stbi_uc> GetByteData() const
        {
            std::vector<stbi_uc> data;
            data.reserve(Width * Height * 3);

            for (int i = 0; i < Width * Height; i++)
            {
                auto& v = Data[i];
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
            Data = newData;
            Width = newWidth;
            Height = newHeight;
        }

        size_t Width = 0, Height = 0;
        bool IsHDR = false;
        Vector3f* Data = nullptr;
    };

    CubeUV XYZToCubeUV(const Vector3f& p);
    Vector3f CubeUVToVector(const CubeUV& cube);
    Vector3f sRGBToHDR(const Vector3f& color);
    Vector3f HDRTosRGB(const Vector3f& hdr);
}