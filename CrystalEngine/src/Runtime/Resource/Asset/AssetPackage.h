#pragma once
#include "AssetCommon.h"
#include "Platform/RHI/FileSystem/FSCommon.h"
#include <Core/Utils/Misc.h>

#include "Content/Fonts/FontFamily.h"
#include "Content/CPUTexture/CPUTexture2D.h"
#include "Content/CPUTexture/CPUTextureCubemap.h"
#include "Function/Framework/Components/Mesh/Mesh.h"

namespace crystal
{
    class AssetPackage
    {
    public:
        AssetPackage(const std::string& name);
        ~AssetPackage();

        void LoadShaders(const path_type& parentPath, const std::vector<std::string>& entries);
        void LoadTextures(const path_type& parentPath, const std::vector<std::string>& entries);
        void LoadCPUTextures(const path_type& parentPath, const std::vector<std::string>& entries);
        void LoadMeshes(const path_type& parentPath, const std::vector<std::string>& entries);
        // void LoadFonts(const std::vector<path_type>& paths);

        void LoadOneShader();
        void LoadOneFont(std::string name, std::shared_ptr<FontFamily> font);
        void LoadOneTexture2D(std::string name, std::shared_ptr<ITexture2D> texture);

        std::shared_ptr<IShaderProgram> GetShader(const URI& uri) const;
        std::shared_ptr<ITexture2D> GetTexture2D(const URI& uri) const;
        std::shared_ptr<ITextureCubemap> GetTextureCubemap(const URI& uri) const;
        std::shared_ptr<FontFamily> GetFontFamily(const URI& uri) const;
        std::shared_ptr<Mesh> GetMesh(const URI& uri) const;
        std::shared_ptr<CPUTexture2D> GetCPUTexture2D(const URI& uri) const;
        std::shared_ptr<CPUTextureCubemap> GetCPUTextureCubemap(const URI& uri) const;

    private:
        std::string   m_packageName;
        std::map<std::string, std::shared_ptr<FontFamily>>      m_fontMap;
        std::map<std::string, std::shared_ptr<IShaderProgram>>  m_shadersMap;
        std::map<std::string, std::shared_ptr<ITexture2D>>      m_texture2DMap;
        std::map<std::string, std::shared_ptr<ITextureCubemap>> m_textureCubeMap;
        std::map<std::string, std::shared_ptr<Mesh>>            m_meshesMap;

        std::map<std::string, std::shared_ptr<CPUTexture2D>>        m_cpu_texture2DMap{};
        std::map<std::string, std::shared_ptr<CPUTextureCubemap>>   m_cpu_textureCubemap{};
        // std::map<std::string, std::shared_ptr<CPUTexture2D>>    m_cpu_texture2DMap;
    };
}