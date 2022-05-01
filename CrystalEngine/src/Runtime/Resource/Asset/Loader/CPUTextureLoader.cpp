#include "CPUTextureLoader.h"

namespace crystal
{
    std::shared_ptr<CPUTexture2D> crystal::CPUTextureLoader::LoadTexture2D(const CPUTexture2DMetaInfo& metaData,
        const path_type& curPath)
    {
        auto pathToTarget = curPath / metaData.Target;
        int width, height, channels;
        if (metaData.TargetComponentFormat == DataFormat::Float32)
        {
            float* data = stbi_loadf(pathToTarget.string().c_str(), &width, &height, &channels, metaData.TargetComponents);
            if (!data)
            {
                throw std::logic_error("Cannot load given image");
            }
            auto ptr = std::make_shared<CPUImageTexture2D>(width, height, data);
            stbi_image_free(data);
            return ptr;
        }
        else
        {
            stbi_uc* data = stbi_load(pathToTarget.string().c_str(), &width, &height, &channels, metaData.TargetComponents);
            if (!data)
            {
                throw std::logic_error("Cannot load given image");
            }
            auto ptr = std::make_shared<CPUImageTexture2D>(width, height, data, metaData.IsHDR);
            stbi_image_free(data);
            return ptr;
        }
    }
    std::shared_ptr<CPUTextureCubemap> CPUTextureLoader::LoadCubemap(const CPUTextureCubemapMetaInfo& metaData, const path_type& curPath)
    {

        CPUTexture2DMetaInfo singleTextureMeta = {};
        singleTextureMeta.TargetComponentFormat = metaData.TargetComponentFormat;
        singleTextureMeta.TargetComponents = metaData.TargetComponents;
        singleTextureMeta.InternalComponentFormat = metaData.InternalComponentFormat;
        singleTextureMeta.InternalComponents = metaData.InternalComponents;
        singleTextureMeta.IsHDR = metaData.IsHDR;
        singleTextureMeta.MipmapLevels = metaData.MipmapLevels;

        CPUCubemapTexture6 cubemapTexs;

        singleTextureMeta.Target = metaData.PosX;
        cubemapTexs.posX = LoadTexture2D(singleTextureMeta, curPath);

        singleTextureMeta.Target = metaData.NegX;
        cubemapTexs.negX = LoadTexture2D(singleTextureMeta, curPath);

        singleTextureMeta.Target = metaData.PosY;
        cubemapTexs.posY = LoadTexture2D(singleTextureMeta, curPath);

        singleTextureMeta.Target = metaData.NegY;
        cubemapTexs.negY = LoadTexture2D(singleTextureMeta, curPath);

        singleTextureMeta.Target = metaData.PosZ;
        cubemapTexs.posZ = LoadTexture2D(singleTextureMeta, curPath);

        singleTextureMeta.Target = metaData.NegZ;
        cubemapTexs.negZ = LoadTexture2D(singleTextureMeta, curPath);

        return std::make_shared<CPUTextureCubemap>(cubemapTexs);
    }
}
