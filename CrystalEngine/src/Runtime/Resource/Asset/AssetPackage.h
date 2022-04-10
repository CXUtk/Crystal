#pragma once
#include "AssetCommon.h"
#include "Platform/RHI/FileSystem/FSCommon.h"
#include <Core/Utils/Misc.h>

#include "Content/Font.h"

namespace crystal
{
    class AssetPackage
    {
    public:
        AssetPackage();
        ~AssetPackage();

        void LoadShaders(const std::vector<path_type>& paths);
        void LoadTextures(const std::vector<path_type>& paths);

        void LoadOneShader();
        void LoadOneFont(std::string name, std::shared_ptr<Font> font);
        void LoadOneTexture2D(std::string name, std::shared_ptr<ITexture2D> texture);

        std::shared_ptr<IShaderProgram> GetShader(const URI& uri) const;
        std::shared_ptr<ITexture2D> GetTexture2D(const URI& uri) const;
        std::shared_ptr<Font> GetFont(const URI& uri) const;

    private:
        std::map<std::string, std::shared_ptr<Font>>            m_fontMap;
        std::map<std::string, std::shared_ptr<IShaderProgram>>  m_shadersMap;
        std::map<std::string, std::shared_ptr<ITexture2D>>      m_texture2DMap;
    };
}