#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include "Geometry.h"

namespace crystal
{
	struct ImageInfo
	{
		Vector2i	Size;
		int			Channels;

		std::shared_ptr<Byte[]>		Data;
	};

	struct LoadDescription
	{
		bool	FlipVertical;
	};

	class IOUtils
	{
	public:
		static bool ReadImage(const char* filename, const LoadDescription& loadDesc, ImageInfo* outputImg);
	};
}