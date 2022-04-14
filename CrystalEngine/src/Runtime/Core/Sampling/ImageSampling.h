#pragma once
#include <Core/Utils/Math.h>
#include <Core/Utils/Geometry.h>

#include <string>
#include <vector>
#include <stbi/stb_image.h>

namespace crystal
{
    struct CubeUV
    {
        int         Id;
        Vector2f    UV;
    };

    class RawTexture2D
    {
    public:
        RawTexture2D(int width, int height)
            : Width(width), Height(height)
        {
            Data = new Vector3f[Width * Height];
        }

        RawTexture2D(int width, int height, stbi_uc* data)
            : Width(width), Height(height)
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
            }
        }

        ~RawTexture2D()
        {
            delete[] Data;
        }

        Vector3f Sample(Vector2f coord) const
        {
            size_t row = (size_t)((1.0 - coord.y) * Height);
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
                data.push_back((unsigned char)floor(v.r * 255));
                data.push_back((unsigned char)floor(v.g * 255));
                data.push_back((unsigned char)floor(v.b * 255));
            }
            return data;
        }

        size_t Width = 0, Height = 0;
        Vector3f* Data = nullptr;
    };

    CubeUV XYZToCubeUV(const Vector3f& p);
    Vector3f CubeUVToVector(const CubeUV& cube);
    Vector3f sRGBToHDR(const Vector3f& color);
    Vector3f HDRTosRGB(const Vector3f& hdr);
}