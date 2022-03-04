#include "Font.h"

#include <Engine.h>

#include <Core/Utils/Misc.h>

namespace crystal
{
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

    const Character& Font::GetCharacter(uint32_t code) const
    {
        auto p = m_charMap.find(code);
        if (p == m_charMap.end())
        {
            throw std::invalid_argument(string_format("Invalid char code %d\n", code));
        }
        return p->second;
    }

    void Font::GenerateCharMap()
    {
        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(m_fontFace, c, FT_LOAD_RENDER))
            {
                throw std::exception("Failed to load freetype char");
            }
            auto engine = Engine::GetInstance();
            auto graphicsDevice = engine->GetGraphicsDevice();

            auto& bitmap = m_fontFace->glyph->bitmap;
            auto width = bitmap.width;
            auto height = bitmap.rows;

            Texture2DDescription texDesc = {};
            texDesc.Size = Vector2i(width,
                height);
            texDesc.Format = RenderFormat::RGBA8ub;
            texDesc.Usage = BufferUsage::Immutable;
            texDesc.MipmapLevels = 1;

            std::shared_ptr<ITexture2D> texture = nullptr;
            // If the character is valid, then generate texture
            if (bitmap.buffer)
            {
                unsigned char* data = new unsigned char[width * height * 4];
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        int id = (i * width + j);
                        unsigned char r = bitmap.buffer[id];
                        for (int k = 0; k < 4; k++)
                        {
                            data[4 * id + k] = r;
                        }
                    }
                }
                texture = graphicsDevice->CreateTexture2DFromMemory(data,
                 width * height * 4, texDesc);
                delete[] data;
            }

            Character character = {
                texture,
                Vector2i(bitmap.width, bitmap.rows),
                Vector2i(m_fontFace->glyph->bitmap_left, m_fontFace->glyph->bitmap_top),
                m_fontFace->glyph->advance.x
            };
            m_charMap.insert(std::pair<uint32_t, Character>(c, character));
        }
    }
}