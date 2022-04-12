#pragma once
#include <map>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <Platform/RHI/Graphics/GraphicsCommon.h>

namespace crystal
{
    class Font;

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


    class FontFamily
    {
    public:
        FontFamily(FT_Face face);
        ~FontFamily();

        FT_Face GetFontFace() const { return m_fontFace; }
        std::shared_ptr<Font> GetFont(int fontSize);

    private:
        FT_Face m_fontFace = nullptr;
        std::map<int, std::shared_ptr<Font>>    m_fontMap{};
    };
}