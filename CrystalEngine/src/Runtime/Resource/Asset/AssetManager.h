#pragma once
#include "AssetCommon.h"
#include "Platform/RHI/FileSystem/FSCommon.h"/
#include <Core/Utils/Misc.h>

#include "AssetPackage.h"
#include "Loader/FontLoader.h"

namespace crystal
{
    class AssetManager
    {
    public:
        AssetManager();
        ~AssetManager();

        static fs::path GetAssetRootPath();
        static fs::path GetAssetPackageConfigFileName();

        void Initialize();
        void LoadAssetPackage(const std::string& name, const path_type& path);

        template<typename T>
        std::shared_ptr<T> LoadAsset(const URI& uri);
        template<typename T>
        std::shared_ptr<T> LoadAssetBuiltIn(const URI& uri);
    private:
        std::map<std::string, std::shared_ptr<AssetPackage>>    m_packagesMap{};
        std::unique_ptr<FontLoader>     m_pFontLoader = nullptr;

        void LoadBuiltinPackage();
        void LoadExtraPackages();

    };

    inline std::string GetPackageName(const URI& uri)
    {
        auto p = std::find(uri.begin(), uri.end(), ':');
        if (p == uri.end())
        {
            throw std::invalid_argument(string_format("Package name invalid"));
        }
        return uri.substr(0, p - uri.begin());
    }

    inline std::string GetResourcePath(const URI& uri)
    {
        auto p = std::find(uri.begin(), uri.end(), ':');
        int len = p - uri.begin() + 1;
        return uri.substr(len, uri.size() - len);
    }

    template<typename T>
    inline std::shared_ptr<T> AssetManager::LoadAsset(const URI& uri)
    {
        auto package = GetPackageName(uri);
        auto resPath = GetResourcePath(uri);

        auto p = m_packagesMap.find(package);
        if (p == m_packagesMap.end())
        {
            throw std::invalid_argument(string_format("Unknown package name: %s", package.c_str()));
        }

        if constexpr (std::is_same<T, IShaderProgram>::value)
        {
            return p->second->GetShader(resPath);
        }
        else if constexpr (std::is_same<T, ITexture2D>::value)
        {
            return p->second->GetTexture2D(resPath);
        }
        else if constexpr (std::is_same<T, FontFamily>::value)
        {
            return p->second->GetFontFamily(resPath);
        }
        else
        {
            static_assert(false, "AssetManager does not support this asset type");
        }
    }
    template<typename T>
    inline std::shared_ptr<T> AssetManager::LoadAssetBuiltIn(const URI& uri)
    {
        return LoadAsset<T>("engine:" + uri);
    }
}