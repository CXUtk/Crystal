#pragma once
#include <memory>
#include <Interfaces/Interfaces.h>
#include <SJson/SJson.hpp>
#include <Interfaces/FileSystem/File.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace crystal
{
    struct Character
    {
        std::shared_ptr<ITexture2D>     Texture2D;      // Char texture
        Vector2i                        Size;           // Size of glyph
        Vector2i                        Bearing;        // Offset from baseline to left/top of glyph
        uint32_t                        Advance;        // Offset to advance to next glyph
    };

    struct TextMetric
    {
        int Width = 0;
        int yMin = std::numeric_limits<int>::max(), yMax = std::numeric_limits<int>::min();
    };

    class Font
    {
    public:
        Font(FT_Face face, size_t fontSize);
        ~Font();

        Character& GetCharacter(uint32_t code);
        TextMetric MeasureString(const std::string& str);
        std::vector<uint32_t> GetCodesFromString(const std::string& str);
        float GetDescender() const;
    private:
        FT_Face     m_fontFace = nullptr;
        size_t      m_fontSize = 0;

        std::map<uint32_t, Character>   m_charMap;

        void GenerateCharMap();
        void LoadChar(uint32_t code);
    };
}