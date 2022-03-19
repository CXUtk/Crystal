#include "FontLoader.h"
#include <Core/Utils/Logger.h>
#include <Core/Utils/Misc.h>

namespace crystal
{
    FontLoader::FontLoader()
    {
        FT_Error error = 0;
        if (error = FT_Init_FreeType(&m_fontLibrary))
        {
            throw std::runtime_error(string_format("FontLoader(): Could not init FreeType Library. Message: %s\n", FT_Error_String(error)));
        }
    }

    FontLoader::~FontLoader()
    {
        FT_Done_FreeType(m_fontLibrary);
    }

    std::shared_ptr<Font> FontLoader::LoadFont(const path_type& path, int size)
    {
        FT_Face face;

        std::string pathStr = path.string();

        if (FT_New_Face(m_fontLibrary, pathStr.c_str(), 0, &face))
        {
            throw std::runtime_error(string_format("FontLoader::LoadFont(): Failed to load font: %s\n", pathStr.c_str()));
        }
        return std::make_shared<Font>(face, size);
    }
}