#pragma once
#include <memory>
#include "Platform/RHI/Interfaces.h"
#include <SJson/SJson.hpp>
#include "Platform/RHI/FileSystem/File.h"

namespace crystal
{
    class TextureLoader
    {
    public:

        static std::shared_ptr<ITexture2D> LoadTexture2D(const SJson::JsonNode& metaData,
            const path_type& curPath);

        static std::shared_ptr<ITextureCubemap> LoadCubemap(const SJson::JsonNode& metaData,
            const path_type& curPath);

        static std::shared_ptr<ITextureCubemap> LoadCubemapWithMips(const SJson::JsonNode& metaData,
            const path_type& curPath, int mipLevels);
    private:

        static std::shared_ptr<ITexture2D> LoadTexture2DFromFile(const path_type& path, const Texture2DDescription& desc);
        static std::shared_ptr<ITextureCubemap> LoadCubemapFromFile(const std::vector<path_type>& paths, Texture2DDescription& desc);
    };
}