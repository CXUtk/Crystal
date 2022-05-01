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
}
