#pragma once
#include "CPUTexture.h"
#include "CPUTexture2D.h"

namespace crystal
{
    struct CPUCubemapTexture6
    {
        std::shared_ptr<CPUTexture2D> posX;
        std::shared_ptr<CPUTexture2D> negX;
        std::shared_ptr<CPUTexture2D> posY;
        std::shared_ptr<CPUTexture2D> negY;
        std::shared_ptr<CPUTexture2D> posZ;
        std::shared_ptr<CPUTexture2D> negZ;
    };

    class CPUTextureCubemap
    {
    public:
        CPUTextureCubemap(const CPUCubemapTexture6& texture6);
        ~CPUTextureCubemap();
        Vector3f Sample(const Vector3f& dir) const;
        Vector3f SampleMipmap(const Vector3f& dir, float lod) const;

    private:
        std::shared_ptr<CPUTexture2D> m_textures[6];
    };
}