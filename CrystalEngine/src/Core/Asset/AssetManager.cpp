#include "AssetManager.h"
#include <Engine.h>
#include <Interfaces/FileSystem/File.h>
#include <SJson/SJson.hpp>
#include <vector>

#include "Loader/ShaderLoader.h"

namespace crystal
{
	AssetManager::AssetManager()
	{}
	AssetManager::~AssetManager()
	{}
	std::shared_ptr<AssetManager> crystal::AssetManager::LoadAssetPackage(const path_type& path)
	{
		std::shared_ptr<AssetManager> assetManager = std::make_shared<AssetManager>();
		auto contents = SJson::JsonConvert::Parse(File::ReadAllText(path));
		auto parentPath = path.parent_path();

		{
			// Load shaders
			std::vector<path_type> shaderPaths;
			contents["shaders"].foreach([&](const SJson::JsonNode& node) {
				shaderPaths.push_back(parentPath / node.Get<std::string>());
			});
			assetManager->m_LoadShaders(shaderPaths);
		}

		return assetManager;
	}

	void AssetManager::m_LoadShaders(const std::vector<path_type>& paths)
	{
		for (auto& path : paths)
		{
			auto shaderMetaInfo = SJson::JsonConvert::Parse(File::ReadAllText(path));
			auto name = shaderMetaInfo["name"].Get<std::string>();
			m_shadersMap[name] = ShaderLoader::LoadShader(shaderMetaInfo, path.parent_path());
		}
	}
}