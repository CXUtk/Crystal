#pragma once
#include <Crystal.h>
#include "FSCommon.h"

namespace crystal
{
	class Path
	{
	public:
		static path_type ChangeExtension(const path_type& path, const std::string& ext);

		static path_type Combine(const std::vector<path_type>& paths);
		static path_type Combine(const path_type& path1, const path_type& path2);

		static path_type Normalize(const path_type& path);
		

		static constexpr char DirectorySeparatorChar = '/';
		static constexpr const char* const DirectorySeparatorCharAsString = "/";
		static constexpr char PathSeparator = ';';
		static constexpr char VolumeSeparatorChar = ':';
	
	private:
		static path_type CombineNoChecks(const path_type& path1, const path_type& path2);
		static bool IsPathRooted(const path_type& path);
		static path_type NormalizeDirectorySeparators(const path_type& path);
	};
}