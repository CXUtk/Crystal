#pragma once
#include "FSCommon.h"
#include <string>
#include <chrono>
#include <memory>
#include <vector>

namespace crystal
{
	class IFileStream;

	inline FileAccess operator|(FileAccess a, FileAccess b)
	{
		return static_cast<FileAccess>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline FileAccess operator&(FileAccess a, FileAccess b)
	{
		return static_cast<FileAccess>(static_cast<int>(a) & static_cast<int>(b));
	}

	class File
	{
	public:
		static std::shared_ptr<IFileStream> Open(const path_type& path, 
			FileMode fileMode, 
			FileAccess fileAccess);

		static std::string ReadAllText(const path_type& path);
		static void WriteAllText(const path_type& path, const std::string& text);

		static std::vector<std::string> ReadAllLines(const path_type& path);
		static void WriteAllLines(const path_type& path, const std::vector<std::string>& lines);

		static std::vector<Byte> ReadAllBytes(const path_type& path);
		static void WriteAllBytes(const path_type& path, const void* src, size_t size);

		static bool Exists(const path_type& path);

		static void Delete(const path_type& path);
		static void Copy(const path_type& srcFile, const path_type& destFile, 
			CopyOptions options = CopyOptions::None);
		static void Move(const path_type& srcFile, const path_type& destFile);

		static time_stamp GetCreationTime(const path_type& path);
		static time_stamp GetLastAccessTime(const path_type& path);
		static time_stamp GetLastReadTime(const path_type& path);
	};
}