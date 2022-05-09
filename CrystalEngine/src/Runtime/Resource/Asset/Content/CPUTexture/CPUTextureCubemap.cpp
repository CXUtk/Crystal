#include "CPUTextureCubemap.h"

namespace crystal
{
    CPUTextureCubemap::CPUTextureCubemap(const CPUCubemapTexture6& texture6)
    {
        m_textures[0] = texture6.posX;
        m_textures[1] = texture6.negX;
        m_textures[2] = texture6.posY;
        m_textures[3] = texture6.negY;
        m_textures[4] = texture6.posZ;
        m_textures[5] = texture6.negZ;

        //// We need to assume all textures in the cubemap have the same size
        //m_weights[0] = 0;
        //m_avgWeights = 0;
        //for (int i = 0; i < 6; i++)
        //{
        //    m_weights[i + 1] = m_weights[i] + m_textures[i]->AverageWeights();
        //    m_avgWeights += m_textures[i]->AverageWeights();
        //}
        //m_avgWeights /= 6.f;
    }

    CPUTextureCubemap::~CPUTextureCubemap()
    {}

    Vector3f CPUTextureCubemap::Sample(const Vector3f & dir) const
    {
        auto cUV = XYZToCubeUV(dir);
        return m_textures[cUV.Id]->Sample(cUV.UV);
    }

    Vector3f CPUTextureCubemap::SampleMipmap(const Vector3f& dir, float lod) const
    {
        return Sample(dir);
    }

    //Vector3f CPUTextureCubemap::WeightedSampleUV(const Vector2f& sample) const
    //{
    //    float X = sample.x * m_weights[6];
    //    int face = std::upper_bound(m_weights, m_weights + 6 + 1,
    //        X) - m_weights - 1;

    //    Vector2f newSample((X - m_weights[face]) / (m_weights[face + 1] - m_weights[face]), sample.y);
    //    auto uv = m_textures[face]->WeightedSampleUV(newSample);
    //    return glm::normalize(CubeUVToVector(CubeUV{ face, uv }));
    //}

    //Float CPUTextureCubemap::AverageWeights() const
    //{
    //    return m_avgWeights;
    //}

    //Float CPUTextureCubemap::Pdf(const Vector3f& dir) const
    //{
    //    auto& v = Sample(dir);
    //    return ((v.r + v.g + v.b) / 3) / AverageWeights();
    //}
}
