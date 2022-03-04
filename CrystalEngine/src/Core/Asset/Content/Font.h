#pragma once
#include <memory>
#include <Interfaces/Interfaces.h>
#include <SJson/SJson.hpp>
#include <Interfaces/FileSystem/File.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace crystal
{
    class Font
    {
    public:
        Font(FT_Face face);
        Font(FT_Face face, size_t fontSize);
        ~Font();

        std::shared_ptr<Font> GetFontWithSize(size_t fontSize) const;

    private:
        FT_Face     m_fontFace = nullptr;
        size_t      m_fontSize = 0;

        void GenerateCharMap();
    };
}