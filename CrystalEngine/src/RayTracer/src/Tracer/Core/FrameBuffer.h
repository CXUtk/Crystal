#pragma once
#include "Tracer/Config/RenderProperties.h"

#include <Function/Render/RenderExports.h>
#include <Function/Framework/Scene/Scene.h>

namespace tracer
{
    class FrameBuffer
    {
    public:
        FrameBuffer(int width, int height);

        void AddSample(int r, int c, const Vector3f& color, float weight = 1.f);

        int GetWidth() const { return m_bufferSize.x; }
        int GetHeight() const { return m_bufferSize.y; }

        void Clear();

        std::shared_ptr<unsigned char[]> GetImageDataRGB8() const;
        std::shared_ptr<float[]> GetImageDataRGB32F() const;

    private:
        struct Pixel
        {
            Vector3f    Color{};
            float       Weight{};
        };
        std::unique_ptr<Pixel[]>    m_colorBuffer{};
        Vector2i                    m_bufferSize{};
    };
}