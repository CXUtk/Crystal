#include "AssetManager.h"
#include <Engine.h>
#include <Interfaces/FileSystem/File.h>
#include <SJson/SJson.hpp>
namespace crystal
{
	std::shared_ptr<AssetManager> crystal::AssetManager::LoadAssetPackage(const path_type& path)
	{
		auto contents = SJson::JsonConvert::Parse(File::ReadAllText(path));
		return std::shared_ptr<AssetManager>();
	}
}