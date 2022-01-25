#pragma once
#include <Platforms/Platforms.h>
#include <string>
#include <fstream>

namespace crystal
{
	class IFileSystem
	{
	public:
		virtual ~IFileSystem() = 0 {};
	};
}