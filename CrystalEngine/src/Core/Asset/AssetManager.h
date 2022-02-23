#pragma once
#include <memory>
#include <Interfaces/Interfaces.h>
#include <Interfaces/FileSystem/FSCommon.h>

namespace crystal
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		static std::shared_ptr<AssetManager> LoadAssetPackage(const path_type& path);

	private:
	};
}