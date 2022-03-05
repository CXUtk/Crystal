#pragma once
#include <string>

namespace crystal
{

    class EncodingHelper
    {
    public:
        static size_t Utf8CharToUcs4Char(const std::string& utf8_str, size_t cursor, uint32_t& ucs4);
    };
}