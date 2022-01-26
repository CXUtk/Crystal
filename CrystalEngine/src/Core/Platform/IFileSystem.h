#pragma once
#include <string>
#include <fstream>

namespace crystal
{
	enum class FileMode
	{
		Create,
		OpenRead,
		OpenWrite,
	};

	enum class FileAccessMode
	{
		Read,
		Write,
		ReadWrite,
	};

	class IFileSystem
	{
	public:
		virtual ~IFileSystem() = 0 {};
	};
}