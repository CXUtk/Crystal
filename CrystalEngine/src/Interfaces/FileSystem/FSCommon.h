#pragma once
#include <Crystal.h>
#include <string>
#include <chrono>
#include <memory>
#include <fstream>
#include <vector>
namespace crystal
{
	using path_type = std::string;
	using time_stamp = std::chrono::time_point<std::chrono::steady_clock>;

	enum class FileMode
	{
		Open,
		OpenOrCreate,
		Append,
	};

	enum FileAccess : uint32_t
	{
		CRYSTAL_FILE_ACCESS_READ = 1 << 0,
		CRYSTAL_FILE_ACCESS_WRITE = 1 << 1
	};
}