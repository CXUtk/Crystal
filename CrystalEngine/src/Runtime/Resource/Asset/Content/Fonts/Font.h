#pragma once

#include "FontFamily.h"
#include <SJson/SJson.hpp>

namespace crystal
{


    class Font
    {
    public:
        Font(FontFamily* fontFamily, size_t fontSize);
        ~Font();

        Character& GetCharacter(uint32_t code);
        TextMetric MeasureString(const std::string& str);
        TextMetric MeasureString(const std::u32string_view& strView);
        std::vector<float> GetWidthsForAllChars(const std::u32string_view& str);
        std::vector<uint32_t> GetCodesFromString(const std::string& str);
        Bound2f GetBoundingBox() const;
    private:
        FontFamily*             m_fontFamily = nullptr;
        size_t                  m_fontSize = 0;

        std::map<uint32_t, Character>   m_charMap;

        void GenerateCharMap();
        void LoadChar(uint32_t code);
    };
}