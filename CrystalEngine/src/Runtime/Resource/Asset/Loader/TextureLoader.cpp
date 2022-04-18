#include "TextureLoader.h"
#include <Engine.h>
#include <Core/Utils/Misc.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>


namespace crystal
{
    std::shared_ptr<ITexture2D> TextureLoader::LoadTexture2D(const Texture2DMetaInfo& metaData,
        const path_type& curPath)
    {
        auto pathToTarget = curPath / metaData.Target;

        Texture2DDescription texDesc = {};
        texDesc.Format = metaData.Format;
        texDesc.MipmapLevels = 1;
        texDesc.MipmapLevels = metaData.MipmapLevels;
        texDesc.Usage = BufferUsage::Immutable;

        return LoadTexture2DFromFile(pathToTarget, texDesc);
    }

    std::shared_ptr<ITextureCubemap> TextureLoader::LoadCubemap(const TextureCubemapMetaInfo& metaData, const path_type& curPath)
    {
        Texture2DDescription texDesc = {};
        texDesc.Format = metaData.Format;
        texDesc.MipmapLevels = metaData.MipmapLevels;
        texDesc.Usage = BufferUsage::Default;

        std::vector<path_type> paths;
        paths.push_back(curPath / metaData.PosX);
        paths.push_back(curPath / metaData.NegX);
        paths.push_back(curPath / metaData.PosY);
        paths.push_back(curPath / metaData.NegY);
        paths.push_back(curPath / metaData.PosZ);
        paths.push_back(curPath / metaData.NegZ);

        return LoadCubemapFromFile(paths, texDesc);
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

    std::shared_ptr<ITextureCubemap> TextureLoader::LoadCubemapWithMips(const TextureCubemapMetaWithMipsInfo& metaData, const path_type& curPath)
    {
        Texture2DDescription mainTexDesc = {};
        mainTexDesc.Format = metaData.Format;
        mainTexDesc.MipmapLevels = metaData.MipmapLevels;

        // temp assert
        assert(mainTexDesc.MipmapLevels > 1);
        mainTexDesc.Usage = BufferUsage::Default;

        std::vector<path_type> paths;
        paths.push_back(curPath / metaData.PosXPattern);
        paths.push_back(curPath / metaData.NegXPattern);
        paths.push_back(curPath / metaData.PosYPattern);
        paths.push_back(curPath / metaData.NegYPattern);
        paths.push_back(curPath / metaData.PosZPattern);
        paths.push_back(curPath / metaData.NegZPattern);


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
        Texture2DDescription& desc)
    {
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();
        if (!File::Exists(path))
        {
            throw std::runtime_error(string_format("Cannot find given file: %s", path.string()));
        }

        // If is HDR image
        if (desc.Format >= RenderFormat::R32f && desc.Format <= RenderFormat::RGBA32f)
        {
            int width, height, channels;
            auto data = stbi_loadf(path.string().c_str(), &width, &height, &channels, 0);
            if (!data)
            {
                throw std::runtime_error(string_format("Failed to load file: %s", path.string()));
            }

            size_t size = (size_t)width * height * channels * sizeof(float);
            desc.Size = Vector2i(width, height);
            return graphicsDevice->CreateTexture2DFromMemory(data, size, desc);
        }
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