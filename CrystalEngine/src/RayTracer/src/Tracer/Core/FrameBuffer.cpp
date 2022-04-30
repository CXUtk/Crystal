#include "FrameBuffer.h"
#include <Core/Utils/Logger.h>

namespace tracer
{
    FrameBuffer::FrameBuffer(int width, int height)
        : m_bufferSize(width, height)
    {
        m_colorBuffer = std::unique_ptr<Pixel[]>(new Pixel[width * height]);
    }

    void FrameBuffer::AddSample(int r, int c, const Vector3f& color, float weight)
    {
        if (glm::isnan(color) != glm::bvec3(false))
        {
            GlobalLogger::Log(crystal::SeverityLevel::Warning, "FrameBuffer::AddSample NAN detected");
            return;
        }
        if (glm::isinf(color) != glm::bvec3(false))
        {
            GlobalLogger::Log(crystal::SeverityLevel::Warning, "FrameBuffer::AddSample INF detected");
            return;
        }
        r = m_bufferSize.y - 1 - r;
        auto& pixel = m_colorBuffer[r * m_bufferSize.x + c];
        pixel.Color += color * weight;
        pixel.Weight += weight;
    }

    void FrameBuffer::Clear()
    {
        for (int i = 0; i < m_bufferSize.y; i++)
        {
            for (int j = 0; j < m_bufferSize.x; j++)
            {
                m_colorBuffer[i * m_bufferSize.x + j] = Pixel{};
            }
        }
    }

    std::shared_ptr<unsigned char[]> FrameBuffer::GetImageDataRGBA8() const
    {
        auto data = std::shared_ptr<unsigned char[]>(new unsigned char[m_bufferSize.x * m_bufferSize.y * 4]);
        for (int i = 0; i < m_bufferSize.y; i++)
        {
            for (int j = 0; j < m_bufferSize.x; j++)
            {
                int dest = i * m_bufferSize.x + j;
                auto c = m_colorBuffer[dest].Color / m_colorBuffer[dest].Weight;

                c = glm::pow(c, glm::vec3(1 / 2.2));

                data[dest * 4] = (unsigned char)round(glm::clamp(c.r, 0.f, 1.f) * 255);
                data[dest * 4 + 1] = (unsigned char)round(glm::clamp(c.g, 0.f, 1.f) * 255);
                data[dest * 4 + 2] = (unsigned char)round(glm::clamp(c.b, 0.f, 1.f) * 255);
                data[dest * 4 + 3] = 255;
            }
        }
        return data;
    }

    std::shared_ptr<float[]> FrameBuffer::GetImageDataRGB32F() const
    {
        auto data = std::shared_ptr<float[]>(new float[m_bufferSize.x * m_bufferSize.y * 3]);
        for (int i = 0; i < m_bufferSize.y; i++)
        {
            for (int j = 0; j < m_bufferSize.x; j++)
            {
                int dest = i * m_bufferSize.x + j;
                auto c = m_colorBuffer[dest].Color / m_colorBuffer[dest].Weight;

                data[dest * 3] = c.r;
                data[dest * 3 + 1] = c.g;
                data[dest * 3 + 2] = c.b;
            }
        }
        return data;
    }
}