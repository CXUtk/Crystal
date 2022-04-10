#include "IO.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>
#include <Core/Utils/Misc.h>

namespace crystal
{
	bool crystal::IOUtils::ReadImage(const char* filename, const LoadDescription& loadDesc, ImageInfo* outputImg)
	{
		memset(outputImg, 0, sizeof(ImageInfo));

		stbi_set_flip_vertically_on_load(loadDesc.FlipVertical);
		Byte* data = stbi_load(filename, &outputImg->Size.x, &outputImg->Size.y, &outputImg->Channels, 0);
		if (!data)
		{
			throw std::exception(string_format("Cannot load texture %s: %s", filename, stbi_failure_reason()).c_str());
			return false;
		}
		std::shared_ptr<Byte[]> src(data, [](Byte p[]) {
			stbi_image_free(p);
		});
		outputImg->Data = src;
		return true;
	}
}