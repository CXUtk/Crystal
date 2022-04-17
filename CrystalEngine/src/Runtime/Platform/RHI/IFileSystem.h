#pragma once
#include <string>
#include <fstream>

namespace crystal
{
	class IFileSystem
	{
	public:
		virtual ~IFileSystem() = 0;
	};
}