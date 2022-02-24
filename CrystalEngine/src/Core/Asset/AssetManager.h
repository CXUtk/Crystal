#pragma once
#include <memory>
#include <Interfaces/Interfaces.h>
#include <Interfaces/FileSystem/FSCommon.h>
#include <Core/Utils/Misc.h>

namespace crystal
{
	using URI = std::string;

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		static std::shared_ptr<AssetManager> LoadAssetPackage(const path_type& path);

		template<typename T>
		std::shared_ptr<T> LoadAsset(const URI& uri);
	private:
		std::map<std::string, std::shared_ptr<IShaderProgram>> m_shadersMap;

		void m_LoadShaders(const std::vector<path_type>& paths);
	};

	template<typename T>
	inline std::shared_ptr<T> AssetManager::LoadAsset(const URI& uri)
	{
		if constexpr (std::is_same<T, IShaderProgram>::value)
		{
			auto p = m_shadersMap.find(uri);
			if (p == m_shadersMap.end())
			{
				throw std::invalid_argument(string_format("Cannot find given shader asset: %s", uri.c_str()));
			}
			return p->second;
		}
		else
		{
			static_assert(false, "AssetManager does not support this asset type");
		}
	}
}