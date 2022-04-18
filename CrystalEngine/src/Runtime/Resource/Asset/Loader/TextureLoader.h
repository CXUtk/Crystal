#pragma once
#include <memory>
#include "Platform/RHI/Interfaces.h"
#include <SJson/SJson.hpp>
#include "Platform/RHI/FileSystem/File.h"
#include "Resource/Asset/AssetMeta/AssetMetaInfo.h"

namespace crystal
{
    class TextureLoader
    {
    public:

        static std::shared_ptr<ITexture2D> LoadTexture2D(const Texture2DMetaInfo& metaData,
            const path_type& curPath);

        static std::shared_ptr<ITextureCubemap> LoadCubemap(const TextureCubemapMetaInfo& metaData,
            const path_type& curPath);

        static std::shared_ptr<ITextureCubemap> LoadCubemapWithMips(const TextureCubemapMetaWithMipsInfo& metaData,
            const path_type& curPath);
    private:

        static std::shared_ptr<ITexture2D> LoadTexture2DFromFile(const path_type& path, Texture2DDescription& desc);
        static std::shared_ptr<ITextureCubemap> LoadCubemapFromFile(const std::vector<path_type>& paths, Texture2DDescription& desc);
    };
}