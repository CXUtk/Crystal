#include "AssetPackage.h"
#include <SJson/SJson.hpp>
#include <vector>

#include "Loader/ShaderLoader.h"
#include "Loader/TextureLoader.h"
#include "Loader/ObjLoader.h"

namespace crystal
{
    AssetPackage::AssetPackage(const std::string& name)
        : m_packageName(name)
    {}

    AssetPackage::~AssetPackage()
    {}

    void AssetPackage::LoadShaders(const path_type& parentPath, const std::vector<std::string>& entries)
    {
        for (auto& name : entries)
        {
            auto path = parentPath / (name + ".json");
            auto shaderMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
            if (m_shadersMap.find(name) != m_shadersMap.end())
            {
                throw std::logic_error(string_format("Asset Name Conflict: Shader %s already exist", name.c_str()));
            }
            m_shadersMap[name] = ShaderLoader::LoadShader(shaderMetaInfo, path.parent_path());
        }
    }

    void AssetPackage::LoadTextures(const path_type& parentPath, const std::vector<std::string>& entries)
    {
        for (auto& name : entries)
        {
            auto path = parentPath / (name + ".json");

            auto textureMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
            auto type = textureMetaInfo["type"].Get<std::string>();

            if (type == "Texture2D")
            {
                if (m_texture2DMap.find(name) != m_texture2DMap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: Texture2D %s already exist", name.c_str()));
                }
                m_texture2DMap[name] = TextureLoader::LoadTexture2D(textureMetaInfo, path.parent_path());
            }
            else if (type == "Cubemap")
            {
                if (m_textureCubeMap.find(name) != m_textureCubeMap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: Cubemap %s already exist", name.c_str()));
                }
                m_textureCubeMap[name] = TextureLoader::LoadCubemap(textureMetaInfo, path.parent_path());
            }   
        }
    }

    void AssetPackage::LoadMeshes(const path_type& parentPath, const std::vector<std::string>& entries)
    {
        ObjLoader loader;
        for (auto& name : entries)
        {
            auto path = parentPath / (name + ".json");
            auto meshMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
            if (m_meshesMap.find(name) != m_meshesMap.end())
            {
                throw std::logic_error(string_format("Asset Name Conflict: Mesh %s already exist", name.c_str()));
            }

            auto target = meshMetaInfo["target"].Get<std::string>();
            auto pathToTarget = path.parent_path() / target;
            loader.load(pathToTarget);

            m_meshesMap[name] = loader.GetMesh();
        }
    }

    void AssetPackage::LoadOneFont(std::string name, std::shared_ptr<FontFamily> font)
    {
        auto p = m_fontMap.find(name);
        if (p != m_fontMap.end())
        {
            throw std::invalid_argument(string_format("Font %s already existed", name.c_str()));
        }
        m_fontMap[name] = font;
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
    std::shared_ptr<FontFamily> AssetPackage::GetFontFamily(const URI& uri) const
    {
        auto p = m_fontMap.find(uri);
        if (p == m_fontMap.end())
        {
            throw std::invalid_argument(string_format("Cannot find given font asset: %s", uri.c_str()));
        }
        return p->second;
    }
    std::shared_ptr<Mesh> AssetPackage::GetMesh(const URI& uri) const
    {
        auto p = m_meshesMap.find(uri);
        if (p == m_meshesMap.end())
        {
            throw std::invalid_argument(string_format("Cannot find given mesh asset: %s", uri.c_str()));
        }
        return p->second;
    }
}
