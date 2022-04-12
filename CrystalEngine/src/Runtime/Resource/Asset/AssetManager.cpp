#include "AssetManager.h"
#include <Engine.h>
#include "Platform/RHI/FileSystem/FSCommon.h"
#include <SJson/SJson.hpp>
#include <vector>

#include "Loader/ShaderLoader.h"

#include "AssetPackage.h"


namespace crystal
{
    static std::string ShadersDirectoryName = "shaders";
    static std::string FontsDirectoryName = "fonts";
    static std::string TexturesDirectoryName = "textures";
    static std::string MeshesDirectoryName = "meshes";


    AssetManager::AssetManager()
    {
        m_pFontLoader = std::make_unique<FontLoader>();
    }

    AssetManager::~AssetManager()
    {
        m_packagesMap.clear();
        m_pFontLoader.reset();
    }

    fs::path AssetManager::GetAssetRootPath()
    {
        return fs::path("assets/");
    }

    fs::path AssetManager::GetAssetPackageConfigFileName()
    {
        return fs::path("contents.json");
    }

    void AssetManager::Initialize()
    {
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();

        LoadBuiltinPackage();
        LoadExtraPackages();

        //Texture2DDescription texDesc = {};
        //texDesc.Format = RenderFormat::RGBA8ub;
        //texDesc.MipmapLevels = 1;
        //texDesc.Usage = BufferUsage::Immutable;
        //assetPackage->LoadOneTexture2D("white",
        //    graphicsDevice->CreateTexture2DFromFile("resources/White.png", texDesc));

        //assetPackage->LoadOneFont("Consolas24", m_pFontLoader->LoadFont("resources/fonts/consola.ttf", 24));
        //assetPackage->LoadOneFont("Consolas18", m_pFontLoader->LoadFont("resources/fonts/consola.ttf", 18));
        //assetPackage->LoadOneFont("Consolas14", m_pFontLoader->LoadFont("resources/fonts/consola.ttf", 14));
        //assetPackage->LoadOneFont("Consolas12", m_pFontLoader->LoadFont("resources/fonts/consola.ttf", 12));

        //m_packagesMap["Crystal"] = assetPackage;

        //m_pAssetManager->LoadAssetPackage("resources/package1/contents.json");
    }

    void AssetManager::LoadAssetPackage(const std::string& name, const path_type& path)
    {
        std::shared_ptr<AssetPackage> assetPackage = std::make_shared<AssetPackage>(name);
        auto contents = SJson::JsonConvert::Parse(File::ReadAllText(path));
        auto parentPath = path.parent_path();

        if(contents.Contains(ShadersDirectoryName))
        {
            // Load shaders
            std::vector<std::string> shaderEntries;
            contents[ShadersDirectoryName].foreach([&](const SJson::JsonNode& node) {                    
                shaderEntries.push_back(node.Get<std::string>());
            });
            assetPackage->LoadShaders(parentPath / ShadersDirectoryName, shaderEntries);
        }

        if(contents.Contains(TexturesDirectoryName))
        {
            // Load textures
            std::vector<std::string> textureEntries;
            contents[TexturesDirectoryName].foreach([&](const SJson::JsonNode& node) {
                textureEntries.push_back(node.Get<std::string>());
            });
            assetPackage->LoadTextures(parentPath / TexturesDirectoryName, textureEntries);
        }

        if (contents.Contains(FontsDirectoryName))
        {
            // Load fonts
            contents[FontsDirectoryName].foreach([&](const SJson::JsonNode& node) {
                auto fileName = node.Get<std::string>();
                auto path = parentPath / FontsDirectoryName / fileName;
                auto name = RemoveExtension(fileName);
                assetPackage->LoadOneFont(name, m_pFontLoader->LoadFontFamily(path));
            });
        }

        if (contents.Contains(MeshesDirectoryName))
        {
            // Load meshes
            std::vector<path_type> meshPaths;
            contents[MeshesDirectoryName].foreach([&](const SJson::JsonNode& node) {
                auto path = parentPath / node.Get<std::string>();
                path.append(".json");

                meshPaths.push_back(path);
            });
            assetPackage->LoadMeshes(meshPaths);
        }

        m_packagesMap[name] = assetPackage;
    }

    void AssetManager::LoadBuiltinPackage()
    {
        const auto& initArgs = Engine::GetInstance()->GetConfigManager()->GetAppInitArgs();
        auto builtinPackagePath = GetAssetRootPath() / initArgs.BuiltinPackage;
        auto builtinPackageConfigPath = builtinPackagePath / GetAssetPackageConfigFileName();

        LoadAssetPackage(initArgs.BuiltinPackage, builtinPackageConfigPath);

        // Additional?
    }

    void AssetManager::LoadExtraPackages()
    {
        const auto& initArgs = Engine::GetInstance()->GetConfigManager()->GetAppInitArgs();

        for (auto& package : initArgs.ExtraPackages)
        {
            auto packageConfigPath = GetAssetRootPath() / package / GetAssetPackageConfigFileName();
            LoadAssetPackage(package, packageConfigPath);
        }
    }

}