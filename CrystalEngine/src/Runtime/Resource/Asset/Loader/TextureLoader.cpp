#include "TextureLoader.h"
#include <Engine.h>

namespace crystal
{
    std::shared_ptr<ITexture2D> TextureLoader::LoadTexture2D(const SJson::JsonNode& metaData, const path_type& curPath)
    {
        auto target = metaData["target"].Get<std::string>();
        auto pathToTarget = curPath / target;

        Texture2DDescription texDesc = {};
        texDesc.Format = SRefl::EnumInfo<RenderFormat>::string_to_enum(metaData["format"].Get<std::string>());
        texDesc.MipmapLevels = 1;

        if (metaData.Contains("mipmapLevels"))
        {
            texDesc.MipmapLevels = metaData["mipmapLevels"].Get<int>();
        }
        texDesc.Usage = BufferUsage::Immutable;

        return LoadTexture2DFromFile(pathToTarget, texDesc);
    }
    std::shared_ptr<ITextureCubemap> TextureLoader::LoadCubemap(const SJson::JsonNode& metaData, const path_type& curPath)
    {
        Texture2DDescription texDesc = {};
        texDesc.Format = SRefl::EnumInfo<RenderFormat>::string_to_enum(metaData["format"].Get<std::string>());
        texDesc.MipmapLevels = 1;
        if (metaData.Contains("mipmapLevels"))
        {
            texDesc.MipmapLevels = metaData["mipmapLevels"].Get<int>();
        }
        texDesc.Usage = BufferUsage::Default;

        CubemapTexture6 cubemapTexs;

        auto target = metaData["posx"].Get<std::string>();
        cubemapTexs.posX = LoadTexture2DFromFile(curPath / target, texDesc);

        texDesc.Size = cubemapTexs.posX->GetSize();

        target = metaData["negx"].Get<std::string>();
        cubemapTexs.negX = LoadTexture2DFromFile(curPath / target, texDesc);

        target = metaData["posy"].Get<std::string>();
        cubemapTexs.posY = LoadTexture2DFromFile(curPath / target, texDesc);

        target = metaData["negy"].Get<std::string>();
        cubemapTexs.negY = LoadTexture2DFromFile(curPath / target, texDesc);

        target = metaData["posz"].Get<std::string>();
        cubemapTexs.posZ = LoadTexture2DFromFile(curPath / target, texDesc);

        target = metaData["negz"].Get<std::string>();
        cubemapTexs.negZ = LoadTexture2DFromFile(curPath / target, texDesc);

        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();
        return graphicsDevice->CreateCubemapFromTexture6(cubemapTexs, texDesc);
    }
    std::shared_ptr<ITexture2D> TextureLoader::LoadTexture2DFromFile(const path_type& path,
        const Texture2DDescription& desc)
    {
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();
        return graphicsDevice->CreateTexture2DFromFile(path.string(), desc);
    }
}