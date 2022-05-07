#include "CPUTexture2D.h"

namespace crystal
{
    Vector3f CPUTexture2DCheckerBoard::Sample(const Vector2f& uv) const
    {
        Point2f uv2 = glm::modf(uv * 10.f, glm::vec2(1.f));
        bool a = uv2.x < 0.5f, b = uv2.y < 0.5f;
        if (a ^ b) return m_W;
        return m_B;
    }

    Vector2f CPUTexture2DCheckerBoard::WeightedSampleUV(const Vector2f& sample) const
    {
        return Vector2f();
    }

    Float CPUTexture2DCheckerBoard::Pdf(const Vector2f& u) const
    {
        auto& v = Sample(u);
        return ((v.r + v.g + v.b) / 3) / AverageWeights();
    }

    CPUImageTexture2D::CPUImageTexture2D(int width, int height, bool isHDR)
        : m_width(width), m_height(height), m_isHDR(isHDR)
    {
        m_data = new Vector3f[m_width * m_height];
        memset(m_data, 0, m_width * m_height * sizeof(Vector3f));
    }

    CPUImageTexture2D::CPUImageTexture2D(int width, int height, float* data)
        : m_width(width), m_height(height), m_isHDR(true)
    {
        m_data = new Vector3f[m_width * m_height];
        memcpy(m_data, data, m_width * m_height * sizeof(Vector3f));
        SetUpSampler();
    }

    CPUImageTexture2D::CPUImageTexture2D(int width, int height, stbi_uc* data, bool isHDR)
        : m_width(width), m_height(height), m_isHDR(isHDR)
    {
        m_data = new Vector3f[m_width * m_height];

        int channels = 3;
        for (int i = 0; i < m_width * m_height; i++)
        {
            size_t row = i / m_width;
            size_t col = i % m_width;
            size_t srcId = (row * m_width + col) * 3;
            m_data[i].r = data[srcId] / 255.f;
            m_data[i].g = data[srcId + 1] / 255.f;
            m_data[i].b = data[srcId + 2] / 255.f;

            if (!m_isHDR)
            {
                m_data[i] = sRGBToHDR(m_data[i]);
            }
        }
        SetUpSampler();
    }

    CPUImageTexture2D::~CPUImageTexture2D()
    {
        delete[] m_data;
        m_data = nullptr;
    }

    Vector3f CPUImageTexture2D::Sample(const Vector2f& uv) const
    {
        // Clamp
        //if (uv.x < 0 || uv.y < 0 || uv.x >= 1.0 || uv.y >= 1.0) return Vector3f(0.f);
        float U = std::fmod(std::fmod(uv.x, 1.0f) + 1.0f, 1.0f);
        float V = std::fmod(std::fmod(uv.y, 1.0f) + 1.0f, 1.0f);
        size_t row = (size_t)(std::min(1.0f - V, 0.9999f) * m_height);
        size_t col = (size_t)(std::min(U, 0.9999f) * m_width);
        return m_data[row * m_width + col];
    }

    Vector3f CPUImageTexture2D::SampleMipmap(const Vector2f& uv, float lod) const
    {
        return Sample(uv);
    }

    Vector3f CPUImageTexture2D::Sample(const Vector2i& coord) const
    {
        assert(coord.x >= 0 && coord.x < m_width&& coord.y >= 0 && coord.y < m_height);
        size_t row = m_height - coord.y - 1;
        return m_data[row * m_width + coord.x];
    }

    void CPUImageTexture2D::SetPixel(const Vector2i& coord, const Vector3f& value)
    {
        assert(coord.x >= 0 && coord.x < m_width&& coord.y >= 0 && coord.y < m_height);
        size_t row = m_height - coord.y - 1;
        assert(value.x <= 1.1 && value.y <= 1.1 && value.z <= 1.1);
        m_data[row * m_width + coord.x] = value;
    }

    void CPUImageTexture2D::SetPixel(const Vector2f& uv, const Vector3f& value)
    {
        assert(value.x <= 1 && value.y <= 1 && value.z <= 1);
        size_t row = (size_t)(std::min(1.0f - uv.y, 0.9999f) * m_height);
        size_t col = (size_t)(std::min(uv.x, 0.9999f) * m_width);
        m_data[row * m_width + col] = value;
    }

    std::vector<stbi_uc> CPUImageTexture2D::GetByteData() const
    {
        std::vector<stbi_uc> data;
        data.reserve(m_width * m_height * 3);

        for (int i = 0; i < m_width * m_height; i++)
        {
            if (!m_isHDR)
            {
                m_data[i] = HDRTosRGB(m_data[i]);
            }
            Vector3f v = glm::clamp(m_data[i], Vector3f(0.f), Vector3f(1.f));
            data.push_back((unsigned char)round(v.r * 255));
            data.push_back((unsigned char)round(v.g * 255));
            data.push_back((unsigned char)round(v.b * 255));
        }
        return data;
    }
    std::vector<float> CPUImageTexture2D::GetFloatData(int components) const
    {
        std::vector<float> data;
        data.resize(m_width * m_height * components);

        for (int i = 0; i < m_width * m_height; i++)
        {
            auto& v = m_data[i];
            for (int j = 0; j < components; j++)
            {
                data[i * components + j] = v[j];
            }
        }
        return data;
    }

    Float CPUImageTexture2D::AverageWeights() const
    {
        return TotalWeight / (m_width * m_height);
    }

    void CPUImageTexture2D::SetUpSampler()
    {
        WeightForRow = new float[m_height + 1];
        memset(WeightForRow, 0, sizeof(float) * (m_height + 1));
        WeightForCol = new float[(m_width + 1) * m_height];
        memset(WeightForCol, 0, sizeof(float) * ((m_width + 1) * m_height));

        for (size_t i = 0; i < m_height; i++)
        {
            for (size_t j = 0; j < m_width; j++)
            {
                auto& data = m_data[i * m_width + j];
                float w = (data.x + data.y + data.z) / 3.f;
                TotalWeight += w;
                WeightForRow[i + 1] += w;
                WeightForCol[i * (m_width + 1) + j + 1] = w;
            }
            for (size_t j = 1; j <= m_width; j++)
            {
                WeightForCol[i * (m_width + 1) + j] += WeightForCol[i * (m_width + 1) + j - 1];
            }
        }

        for (size_t i = 1; i <= m_height; i++)
        {
            WeightForRow[i] += WeightForRow[i - 1];
        }
    }

    Vector2f CPUImageTexture2D::WeightedSampleUV(const Vector2f& sample) const
    {
        float X = sample.x * WeightForRow[m_height];
        int row = std::upper_bound(WeightForRow, WeightForRow + m_height + 1,
            X) - WeightForRow - 1;

        X = (X - WeightForRow[row]) / (WeightForRow[row + 1] - WeightForRow[row]);

        float* start = WeightForCol + row * (m_width + 1);
        int col = std::upper_bound(start, start + m_width + 1,
            X * start[m_width]) - start - 1;

        int Y = (int)(sample.y * 10000);
        float dx = (Y / 100) / 100.f;
        float dy = (Y % 100) / 100.f;

        return Vector2f((col + dx) / m_width, (m_height - row - 1 + dy) / m_height);
    }

    Float CPUImageTexture2D::Pdf(const Vector2f& u) const
    {
        auto& v = Sample(u);
        return ((v.r + v.g + v.b) / 3) / AverageWeights();
    }
}
