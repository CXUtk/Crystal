#pragma once
#include "AssetCommon.h"
#include <Interfaces/FileSystem/FSCommon.h>
#include <Core/Utils/Misc.h>

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
        void LoadOneTexture2D(std::string name, std::shared_ptr<ITexture2D> texture);

        std::shared_ptr<IShaderProgram> GetShader(const URI& uri) const;
        std::shared_ptr<ITexture2D> GetTexture2D(const URI& uri) const;

    private:
        std::map<std::string, std::shared_ptr<IShaderProgram>> m_shadersMap;
        std::map<std::string, std::shared_ptr<ITexture2D>> m_texture2DMap;
    };
}