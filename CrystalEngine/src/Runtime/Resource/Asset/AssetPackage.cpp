#include "AssetPackage.h"
#include <SJson/SJson.hpp>
#include <vector>

#include "Loader/ShaderLoader.h"
#include "Loader/TextureLoader.h"
#include "Loader/MeshLoader.h"
//#include "Loader/ObjLoader.h"
#include "Loader/CPUTextureLoader.h"
#include "AssetMeta/AssetMetaInfo.h"

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
            const auto shaderMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
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

            const auto textureMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
            auto type = textureMetaInfo["type"].Get<std::string>();

            if (textureMetaInfo.Contains("version"))
            {
                // Version back-compatibility
            }

            if (type == "Texture2D")
            {
                if (m_texture2DMap.find(name) != m_texture2DMap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: Texture2D %s already exist", name.c_str()));
                }
                auto meta = SJson::de_serialize<Texture2DMetaInfo>(textureMetaInfo["info"]);
                m_texture2DMap[name] = TextureLoader::LoadTexture2D(meta, path.parent_path());
            }
            else if (type == "Cubemap")
            {
                if (m_textureCubeMap.find(name) != m_textureCubeMap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: Cubemap %s already exist", name.c_str()));
                }
                auto meta = SJson::de_serialize<TextureCubemapMetaInfo>(textureMetaInfo["info"]);
                m_textureCubeMap[name] = TextureLoader::LoadCubemap(meta, path.parent_path());
            }
            else if (type == "Cubemap_mips")
            {
                if (m_textureCubeMap.find(name) != m_textureCubeMap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: Cubemap %s already exist", name.c_str()));
                }
                auto meta = SJson::de_serialize<TextureCubemapMetaWithMipsInfo>(textureMetaInfo["info"]);
                m_textureCubeMap[name] = TextureLoader::LoadCubemapWithMips(meta, path.parent_path());
            }
        }
    }

    void AssetPackage::LoadCPUTextures(const path_type& parentPath, const std::vector<std::string>& entries)
    {
        for (auto& name : entries)
        {
            auto path = parentPath / (name + ".json");

            const auto cputextureMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
            auto type = cputextureMetaInfo["type"].Get<std::string>();

            if (cputextureMetaInfo.Contains("version"))
            {
                // Version back-compatibility
            }

            if (type == "Texture2D")
            {
                if (m_cpu_texture2DMap.find(name) != m_cpu_texture2DMap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: CPU Texture2D %s already exist", name.c_str()));
                }
                auto meta = SJson::de_serialize<CPUTexture2DMetaInfo>(cputextureMetaInfo["info"]);
                m_cpu_texture2DMap[name] = CPUTextureLoader::LoadTexture2D(meta, path.parent_path());
            }
            else if (type == "Cubemap")
            {
                if (m_cpu_textureCubemap.find(name) != m_cpu_textureCubemap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: CPU Cubemap %s already exist", name.c_str()));
                }
                auto meta = SJson::de_serialize<CPUTextureCubemapMetaInfo>(cputextureMetaInfo["info"]);
                m_cpu_textureCubemap[name] = CPUTextureLoader::LoadCubemap(meta, path.parent_path());
            }
            else if (type == "Pure")
            {
                if (m_cpu_texture2DMap.find(name) != m_cpu_texture2DMap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: CPU Texture2D %s already exist", name.c_str()));
                }
                auto meta = SJson::de_serialize<CPUTexture2DPureMetaInfo>(cputextureMetaInfo["info"]);
                m_cpu_texture2DMap[name] = CPUTextureLoader::LoadTexture2DPure(meta, path.parent_path());
            }
            else if (type == "Cubemap_mips")
            {
               /* if (m_textureCubeMap.find(name) != m_textureCubeMap.end())
                {
                    throw std::logic_error(string_format("Asset Name Conflict: Cubemap %s already exist", name.c_str()));
                }
                auto meta = SJson::de_serialize<TextureCubemapMetaWithMipsInfo>(textureMetaInfo["info"]);
                m_textureCubeMap[name] = TextureLoader::LoadCubemapWithMips(meta, path.parent_path());*/
            }
        }
    }

    void AssetPackage::LoadMeshes(const path_type& parentPath, const std::vector<std::string>& entries)
    {
        for (auto& name : entries)
        {
            auto path = parentPath / (name + ".json");
            const auto meshMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
            if (m_meshesMap.find(name) != m_meshesMap.end())
            {
                throw std::logic_error(string_format("Asset Name Conflict: Mesh %s already exist", name.c_str()));
            }

            m_meshesMap[name] = MeshLoader::LoadOneMeshFromObj(meshMetaInfo, parentPath);
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
    std::shared_ptr<ITextureCubemap> AssetPackage::GetTextureCubemap(const URI& uri) const
    {
        auto p = m_textureCubeMap.find(uri);
        if (p == m_textureCubeMap.end())
        {
            throw std::invalid_argument(string_format("Cannot find given cubemap asset: %s", uri.c_str()));
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

    std::shared_ptr<CPUTexture2D> AssetPackage::GetCPUTexture2D(const URI& uri) const
    {
        auto p = m_cpu_texture2DMap.find(uri);
        if (p == m_cpu_texture2DMap.end())
        {
            throw std::invalid_argument(string_format("Cannot find given cpu texture asset: %s", uri.c_str()));
        }
        return p->second;
    }

    std::shared_ptr<CPUTextureCubemap> AssetPackage::GetCPUTextureCubemap(const URI& uri) const
    {
        auto p = m_cpu_textureCubemap.find(uri);
        if (p == m_cpu_textureCubemap.end())
        {
            throw std::invalid_argument(string_format("Cannot find given cpu cubemap texture asset: %s", uri.c_str()));
        }
        return p->second;
    }
}
