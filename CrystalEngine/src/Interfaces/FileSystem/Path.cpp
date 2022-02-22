#include "Path.h"
#include <algorithm>
#include <Core/Utils/Misc.h>

namespace crystal
{
	path_type crystal::Path::Combine(const std::vector<path_type>& paths)
	{
		return path_type();
	}

	path_type Path::Combine(const path_type& path1, const path_type& path2)
	{
		return CombineNoChecks(path1, path2);
	}

	path_type Path::Normalize(const path_type& path)
	{
		return path_type();
	}

	path_type Path::CombineNoChecks(const path_type& path1, const path_type& path2)
	{
		char last = path1.back();
		if (IsPathRooted(path2)) return path2;
		if (last != DirectorySeparatorChar) return path1 + DirectorySeparatorChar + path2;
		return path1 + path2;
	}

	bool Path::IsPathRooted(const path_type& path)
	{
		size_t len = path.length();
		if ((len >= 1 && path[0] == DirectorySeparatorChar) || (len >= 2 && path[1] == VolumeSeparatorChar))
		{
			return true;
		}
		return false;
	}

	path_type Path::NormalizeDirectorySeparators(const path_type& path)
	{
		auto path_str = Trim(path);
		return path_str;
	}
}
