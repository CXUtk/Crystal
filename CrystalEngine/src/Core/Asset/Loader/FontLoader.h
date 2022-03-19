#pragma once
#include <memory>
#include <Interfaces/Interfaces.h>
#include <SJson/SJson.hpp>
#include <Interfaces/FileSystem/File.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Content/Font.h"

namespace crystal
{
	class FontLoader
	{
	public:
		FontLoader();
		~FontLoader();

        std::shared_ptr<Font> LoadFont(const path_type& path, int size);

	private:
		FT_Library	m_fontLibrary = nullptr;
	};
}