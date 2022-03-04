#include "Font.h"

namespace crystal
{
    Font::Font(FT_Face face)
        : m_fontFace(face)
    {}

    Font::Font(FT_Face face, size_t fontSize)
        : m_fontFace(face), m_fontSize(fontSize)
    {
        FT_Set_Pixel_Sizes(face, 0, m_fontSize);
        GenerateCharMap();
    }

    Font::~Font()
    {
        FT_Done_Face(m_fontFace);
    }
    std::shared_ptr<Font> Font::GetFontWithSize(size_t fontSize) const
    {
        return std::make_shared<Font>(m_fontFace, fontSize);
    }
}