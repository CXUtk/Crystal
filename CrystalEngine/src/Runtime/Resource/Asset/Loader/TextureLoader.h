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

    private:
    };
}