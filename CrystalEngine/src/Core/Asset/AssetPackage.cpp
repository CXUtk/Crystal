#include "AssetPackage.h"
#include <SJson/SJson.hpp>
#include <vector>

#include "Loader/ShaderLoader.h"
#include "Loader/TextureLoader.h"

namespace crystal
{
    AssetPackage::AssetPackage()
    {}

    AssetPackage::~AssetPackage()
    {}

    void AssetPackage::LoadShaders(const std::vector<path_type>& paths)
    {
        for (auto& path : paths)
        {
            auto shaderMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
            auto name = shaderMetaInfo["name"].Get<std::string>();
            m_shadersMap[name] = ShaderLoader::LoadShader(shaderMetaInfo, path.parent_path());
        }
    }

    void AssetPackage::LoadTextures(const std::vector<path_type>& paths)
    {
        for (auto& path : paths)
        {
            auto textureMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
            auto name = textureMetaInfo["name"].Get<std::string>();
            m_texture2DMap[name] = TextureLoader::LoadTexture2D(textureMetaInfo, path.parent_path());
        }
    }

    void AssetPackage::LoadOneTexture2D(std::string name, std::shared_ptr<ITexture2D> texture)
    {
        auto p = m_texture2DMap.find(name);
        if (p != m_texture2DMap.end())
        {
            throw std::invalid_argument(string_format("Texture2D %s already existed", name.c_str()));
        }
        m_texture2DMap[name] = texture;
    }

    std::shared_ptr<IShaderProgram> AssetPackage::GetShader(const URI & uri) const
    {
        auto p = m_shadersMap.find(uri);
        if (p == m_shadersMap.end())
        {
            throw std::invalid_argument(string_format("Cannot find given shader asset: %s", uri.c_str()));
        }
        return p->second;
    }

    std::shared_ptr<ITexture2D> AssetPackage::GetTexture2D(const URI& uri) const
    {
        auto p = m_texture2DMap.find(uri);
        if (p == m_texture2DMap.end())
        {
            throw std::invalid_argument(string_format("Cannot find given texture2D asset: %s", uri.c_str()));
        }
        return p->second;
    }
}
