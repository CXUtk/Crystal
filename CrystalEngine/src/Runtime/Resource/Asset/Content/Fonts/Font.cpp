#include "Font.h"

#include <Engine.h>

#include <Core/Utils/Misc.h>
#include <Core/Utils/Encoding.h>

namespace crystal
{
    Font::Font(FontFamily* fontFamily, size_t fontSize)
        : m_fontFamily(fontFamily), m_fontSize(fontSize)
    {
        FT_Error error = 0;
        if (error = FT_Set_Pixel_Sizes(fontFamily->GetFontFace(), 0, m_fontSize))
        {
            throw std::runtime_error(string_format("Font::FT_Set_Pixel_Sizes(): Error setting font size, Message: %s\n", FT_Error_String(error)));
        }
        GenerateCharMap();
    }

    Font::~Font()
    {
    }

    Character& Font::GetCharacter(uint32_t code)
    {
        auto p = m_charMap.find(code);
        if (p == m_charMap.end())
        {
            LoadChar(code);
        }
        return m_charMap[code];
    }

    //const Character& Font::GetCharacter(uint32_t code) const
    //{
    //    auto p = m_charMap.find(code);
    //    if (p == m_charMap.end())
    //    {
    //        throw std::invalid_argument(string_format("Invalid char code %d\n", code));
    //    }
    //    return p->second;
    //}

    TextMetric Font::MeasureString(const std::string& str)
    {
        TextMetric metric = {};

        int len = str.size();
        for (int i = 0; i < len;)
        {
            uint32_t charCode;
            int offset = EncodingHelper::Utf8CharToUcs4Char(str, i, charCode);
            if (!offset)
            {
                throw std::exception("Font::MeasureString: Unsupported char code");
            }
            auto& ch = GetCharacter(charCode);
            metric.Width += ch.Advance;
            metric.yMin = std::min(metric.yMin, ch.Bearing.y - ch.Size.y);
            metric.yMax = std::max(metric.yMax, ch.Bearing.y);

            i += offset;
        }
        return metric;
    }

    TextMetric Font::MeasureString(const std::u32string_view& strView)
    {
        TextMetric metric = {};

        for (auto c : strView)
        {
            auto& ch = GetCharacter(c);
            metric.Width += ch.Advance;
            metric.yMin = std::min(metric.yMin, ch.Bearing.y - ch.Size.y);
            metric.yMax = std::max(metric.yMax, ch.Bearing.y);
        }
        return metric;
    }

    std::vector<float> Font::GetWidthsForAllChars(const std::u32string_view& str)
    {
        std::vector<float> metrics;
        float width = 0.f;
        for (auto c : str)
        {
            auto& ch = GetCharacter(c);
            width += ch.Advance;
            metrics.push_back(width);
        }
        return metrics;
    }

    std::vector<uint32_t> Font::GetCodesFromString(const std::string& str)
    {
        std::vector<uint32_t> res;
        int len = str.size();
        for (int i = 0; i < len;)
        {
            uint32_t charCode;
            int offset = EncodingHelper::Utf8CharToUcs4Char(str, i, charCode);
            if (!offset)
            {
                throw std::exception("Font::MeasureString: Unsupported char code");
            }
            i += offset;
            res.push_back(charCode);
        }
        return res;
    }

    Bound2f Font::GetBoundingBox() const
    {
        auto fontFace = m_fontFamily->GetFontFace();
        auto xMin = std::floor((float)fontFace->bbox.xMin * m_fontSize / fontFace->units_per_EM);
        auto yMin = std::floor((float)fontFace->bbox.yMin * m_fontSize / fontFace->units_per_EM);
        auto xMax = std::ceil((float)fontFace->bbox.xMax * m_fontSize / fontFace->units_per_EM);
        auto yMax = std::ceil((float)fontFace->bbox.yMax * m_fontSize / fontFace->units_per_EM);
        return Bound2f(Vector2f(xMin, yMin), Vector2f(xMax, yMax));
    }

    void Font::GenerateCharMap()
    {
        for (unsigned char c = 0; c < 128; c++)
        {
            LoadChar(c);
        }
    }

    void Font::LoadChar(uint32_t code)
    {
        // load character glyph
        auto fontFace = m_fontFamily->GetFontFace();
        if (FT_Load_Char(fontFace, code, FT_LOAD_RENDER))
        {
            throw std::exception("Font::LoadChar: Failed to load freetype char");
        }
        auto engine = Engine::GetInstance();
        auto graphicsDevice = engine->GetGraphicsDevice();

        auto& bitmap = fontFace->glyph->bitmap;
        auto width = bitmap.width;
        auto height = bitmap.rows;

        Texture2DDescription texDesc = {};
        texDesc.Size = Vector2i(width, height);
        texDesc.Format = RenderFormat::RGBA8ub;
        texDesc.Usage = BufferUsage::Immutable;
        texDesc.MipmapLevels = 1;

        std::shared_ptr<ITexture2D> texture = nullptr;
        // If the character is valid, then generate texture
        if (bitmap.buffer)
        {
            unsigned char* data = new unsigned char[width * height * 4];
            for (size_t i = 0; i < height; i++)
            {
                for (size_t j = 0; j < width; j++)
                {
                    size_t id = (i * width + j);
                    unsigned char r = bitmap.buffer[id];
                    for (int k = 0; k < 3; k++)
                    {
                        data[4 * id + k] = 255;
                    }
                    data[4 * id + 3] = r;
                }
            }
            texture = graphicsDevice->CreateTexture2DFromMemory(data,
             width * height * 4, texDesc);
            delete[] data;
        }
        Character character = {
            texture,
            Vector2i(bitmap.width, bitmap.rows),
            Vector2i(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
            (fontFace->glyph->advance.x >> 6)
        };
        m_charMap.insert(std::pair<uint32_t, Character>(code, character));
    }
}