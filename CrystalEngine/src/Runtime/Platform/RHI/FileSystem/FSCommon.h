#pragma once
#include <Crystal.h>
#include <string>
#include <chrono>
#include <memory>
#include <fstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace crystal
{
	using path_type = fs::path;
	using time_stamp = fs::file_time_type;

	enum class FileMode
	{
		Open,
		OpenOrCreate,
		Append,
	};

	enum class CopyOptions
	{
		None,
		Overwrite,
		SkipExisting,
		Update
	};

	enum FileAccess : uint32_t
	{
		CRYSTAL_FILE_ACCESS_READ = 1 << 0,
		CRYSTAL_FILE_ACCESS_WRITE = 1 << 1
	};
}