#include "TextureLoader.h"
#include <Engine.h>
#include <Core/Utils/Misc.h>

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

        std::vector<path_type> paths;
        paths.push_back(curPath / metaData["posx"].Get<std::string>());
        paths.push_back(curPath / metaData["negx"].Get<std::string>());
        paths.push_back(curPath / metaData["posy"].Get<std::string>());
        paths.push_back(curPath / metaData["negy"].Get<std::string>());
        paths.push_back(curPath / metaData["posz"].Get<std::string>());
        paths.push_back(curPath / metaData["negz"].Get<std::string>());

        return LoadCubemapFromFile(paths, texDesc);
        //CubemapTexture6 cubemapTexs;

        //auto target = metaData["posx"].Get<std::string>();
        //cubemapTexs.posX = LoadTexture2DFromFile(curPath / target, texDesc);

        //texDesc.Size = cubemapTexs.posX->GetSize();

        //target = metaData["negx"].Get<std::string>();
        //cubemapTexs.negX = LoadTexture2DFromFile(curPath / target, texDesc);

        //target = metaData["posy"].Get<std::string>();
        //cubemapTexs.posY = LoadTexture2DFromFile(curPath / target, texDesc);

        //target = metaData["negy"].Get<std::string>();
        //cubemapTexs.negY = LoadTexture2DFromFile(curPath / target, texDesc);

        //target = metaData["posz"].Get<std::string>();
        //cubemapTexs.posZ = LoadTexture2DFromFile(curPath / target, texDesc);

        //target = metaData["negz"].Get<std::string>();
        //cubemapTexs.negZ = LoadTexture2DFromFile(curPath / target, texDesc);

        //auto engine = Engine::GetInstance();
        //auto graphicsDevice = engine->GetGraphicsDevice();
        //return graphicsDevice->CreateCubemapFromTexture6(cubemapTexs, texDesc);
    }


    path_type AddNumberSuffix(const path_type& path, int suffix)
    {
        path_type result = path;
        auto extension = path.extension();
        auto filename = RemoveExtension(path.filename().string());
        filename.append(string_format("_%d%s", suffix, extension.string().c_str()));
        result.replace_filename(filename);
        return result;
    }

    std::vector<path_type> ConvertToSuffixPathList(const std::vector<path_type>& paths, int suffix)
    {
        std::vector<path_type> result;
        for (auto& path : paths)
        {
            result.push_back(AddNumberSuffix(path, suffix));
        }
        return result;
    }

    std::shared_ptr<ITextureCubemap> TextureLoader::LoadCubemapWithMips(const SJson::JsonNode& metaData, const path_type& curPath, int mipLevels)
    {
        Texture2DDescription mainTexDesc = {};
        mainTexDesc.Format = SRefl::EnumInfo<RenderFormat>::string_to_enum(metaData["format"].Get<std::string>());
        mainTexDesc.MipmapLevels = metaData["mipmapLevels"].Get<int>();

        // temp assert
        assert(mainTexDesc.MipmapLevels > 1);
        mainTexDesc.Usage = BufferUsage::Default;

        std::vector<path_type> paths;
        paths.push_back(curPath / metaData["posx"].Get<std::string>());
        paths.push_back(curPath / metaData["negx"].Get<std::string>());
        paths.push_back(curPath / metaData["posy"].Get<std::string>());
        paths.push_back(curPath / metaData["negy"].Get<std::string>());
        paths.push_back(curPath / metaData["posz"].Get<std::string>());
        paths.push_back(curPath / metaData["negz"].Get<std::string>());


        auto mainCubeMap = LoadCubemapFromFile(ConvertToSuffixPathList(paths, 0), mainTexDesc);

        Texture2DDescription texDescSingleLevel = mainTexDesc;
        texDescSingleLevel.MipmapLevels = 1;

        for (int i = 1; i < mainTexDesc.MipmapLevels; i++)
        {
            auto subCubemap = LoadCubemapFromFile(ConvertToSuffixPathList(paths, i), texDescSingleLevel);
            mainCubeMap->ReplaceMipmap(subCubemap, i);
        }

        return mainCubeMap;
    }


    std::shared_ptr<ITexture2D> TextureLoader::LoadTexture2DFromFile(const path_type& path,
        const Texture2DDescription& desc)
    {
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();
        return graphicsDevice->CreateTexture2DFromFile(path.string(), desc);
    }

    std::shared_ptr<ITextureCubemap> TextureLoader::LoadCubemapFromFile(const std::vector<path_type>& paths, Texture2DDescription& desc)
    {
        CubemapTexture6 cubemapTexs;

        cubemapTexs.posX = LoadTexture2DFromFile(paths[0], desc);
        cubemapTexs.negX = LoadTexture2DFromFile(paths[1], desc);
        cubemapTexs.posY = LoadTexture2DFromFile(paths[2], desc);
        cubemapTexs.negY = LoadTexture2DFromFile(paths[3], desc);
        cubemapTexs.posZ = LoadTexture2DFromFile(paths[4], desc);
        cubemapTexs.negZ = LoadTexture2DFromFile(paths[5], desc);

        desc.Size = cubemapTexs.posX->GetSize();

        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();
        return graphicsDevice->CreateCubemapFromTexture6(cubemapTexs, desc);
    }
}