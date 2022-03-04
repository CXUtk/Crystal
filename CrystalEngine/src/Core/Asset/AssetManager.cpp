#include "AssetManager.h"
#include <Engine.h>
#include <Interfaces/FileSystem/File.h>
#include <SJson/SJson.hpp>
#include <vector>

#include "Loader/ShaderLoader.h"

#include "AssetPackage.h"


namespace crystal
{
    AssetManager::AssetManager()
    {
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();

        std::shared_ptr<AssetPackage> assetPackage = std::make_shared<AssetPackage>();

        Texture2DDescription texDesc = {};
        texDesc.Format = RenderFormat::RGBA8ub;
        texDesc.MipmapLevels = 1;
        texDesc.Usage = BufferUsage::Immutable;
        uint32_t pureWhite = 0xffffffff;
        assetPackage->LoadOneTexture2D("white",
            graphicsDevice->CreateTexture2DFromFile("resources/White.png", texDesc));

        m_packagesMap["Crystal"] = assetPackage;


        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        }
F        FT_Error_String

        FT_Face face;
        if (FT_New_Face(ft, "resources/fonts/consola.ttf", 0, &face))
        {
            printf("ERROR::FREETYPE: Failed to load font\n");
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

    }

    AssetManager::~AssetManager()
    {}

    void AssetManager::LoadAssetPackage(const path_type& path)
    {
        std::shared_ptr<AssetPackage> assetPackage = std::make_shared<AssetPackage>();
        auto contents = SJson::JsonConvert::Parse(File::ReadAllText(path));
        auto parentPath = path.parent_path();

        {
            // Load shaders
            std::vector<path_type> shaderPaths;
            contents["shaders"].foreach([&](const SJson::JsonNode& node) {
                shaderPaths.push_back(parentPath / node.Get<std::string>());
            });
            assetPackage->LoadShaders(shaderPaths);
        }

        {
            // Load textures
            std::vector<path_type> texturePaths;
            contents["textures"].foreach([&](const SJson::JsonNode& node) {
                texturePaths.push_back(parentPath / node.Get<std::string>());
            });
            assetPackage->LoadTextures(texturePaths);
        }

        auto s = path.parent_path().stem().string();
        m_packagesMap[s] = assetPackage;
    }

}