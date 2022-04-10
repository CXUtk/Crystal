#pragma once
#include <memory>

#include "Platform/RHI/Interfaces.h"
#include "Platform/RHI/FileSystem/FSCommon.h"

#include <SJson/SJson.hpp>

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