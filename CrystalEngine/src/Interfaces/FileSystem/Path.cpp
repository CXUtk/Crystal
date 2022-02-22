#include "Path.h"
#include <algorithm>
#include <array>
#include <Core/Utils/Misc.h>

namespace crystal
{
	path_type Path::ChangeExtension(const path_type& path, const std::string& ext)
	{
		return path_type();
	}

	path_type crystal::Path::Combine(const std::vector<path_type>& paths)
	{
		std::string path;
		for (auto& p : paths)
		{
			if (IsPathRooted(p))
			{
				path = p;
				continue;
			}
			if (!path.empty() && !p.empty() && path.back() != DirectorySeparatorChar)
			{
				path.push_back(DirectorySeparatorChar);
			}
			path.append(p);
		}
		return path;
	}

	path_type Path::Combine(const path_type& path1, const path_type& path2)
	{
		return CombineNoChecks(path1, path2);
	}

	path_type Path::Normalize(const path_type& path)
	{
		return NormalizeDirectorySeparators(path);
	}

	path_type Path::CombineNoChecks(const path_type& path1, const path_type& path2)
	{
		if (path1.empty()) return path2;
		if (path2.empty()) return path1;
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
		if (std::find(path_str.begin(), path_str.end(), '\\') == path_str.end())
		{
			return path_str;
		}
		size_t len = path_str.size();
		std::string str;
		for (int i = 0; i < len; i++)
		{
			char c = path_str[i];
			if (c == '/' || c == '\\')
			{
				if (i == len - 1 || (path_str[i + 1] != '/' && path_str[i + 1] != '\\'))
				{
					str.push_back(c);
				}
			}
			else
			{
				str.push_back(c);
			}
		}
		return path_str;
	}

	template<size_t N>
	constexpr std::array<bool, N> GenerateInvalidCharTable()
	{
		std::array<bool, N> M{};
		for (int i = 0; i <= 0x1F; i++)
		{
			M[i] = true;
		}
		M['>'] = true;
		M['<'] = true;
		M['|'] = true;
		M['\"'] = true;
		return M;
	}

	static constexpr auto InvalidCharTable = GenerateInvalidCharTable<256UL>();

	void Path::CheckInvalidPathChars(const path_type& path)
	{
		for (auto c : path)
		{
			if (InvalidCharTable[c])
			{
				throw std::invalid_argument("Invalid character in path");
			}
		}
	}
}
