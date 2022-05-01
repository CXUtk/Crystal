#pragma once
#include <memory>
#include "Platform/RHI/Interfaces.h"
#include <SJson/SJson.hpp>
#include "Platform/RHI/FileSystem/File.h"
#include "Resource/Asset/AssetMeta/AssetMetaInfo.h"

#include "Resource/Asset/Content/CPUTexture/CPUTexture2D.h"
#include "Resource/Asset/Content/CPUTexture/CPUTextureCubemap.h"

namespace crystal
{
    class CPUTextureLoader
    {
    public:

        static std::shared_ptr<CPUTexture2D> LoadTexture2D(const CPUTexture2DMetaInfo& metaData,
            const path_type& curPath);

        static std::shared_ptr<CPUTextureCubemap> LoadCubemap(const CPUTextureCubemapMetaInfo& metaData,
            const path_type& curPath);

        /*static std::shared_ptr<ITextureCubemap> LoadCubemapWithMips(const TextureCubemapMetaWithMipsInfo& metaData,
            const path_type& curPath);*/
    private:

        // static std::shared_ptr<ITexture2D> LoadTexture2DFromFile(const path_type& path, Texture2DDescription& desc);
        // static std::shared_ptr<ITextureCubemap> LoadCubemapFromFile(const std::vector<path_type>& paths, Texture2DDescription& desc);
    };
}