#include "Encoding.h"

namespace crystal
{
    size_t EncodingHelper::Utf8CharToUcs4Char(const std::string& utf8_str, size_t cursor, uint32_t& ucs4)
    {
        //We do math, that relies on unsigned data types
        const unsigned char* utf8TokUs = reinterpret_cast<const unsigned char*>(utf8_str.c_str() + cursor);

        //Initialize return values for 'return false' cases.
        ucs4 = 0;

        //Decode
        if (0x80 > utf8TokUs[0])
        {
            //Tokensize: 1 byte
            ucs4 = static_cast<const uint32_t>(utf8TokUs[0]);
            return 1;
        }
        else if (0xC0 == (utf8TokUs[0] & 0xE0))
        {
            // 0xC0 11000000, 0xE0 11100000
            //Tokensize: 2 bytes
            if (0x80 != (utf8TokUs[1] & 0xC0))
            {
                return 0;
            }
            ucs4 = static_cast<const uint32_t>((utf8TokUs[0] & 0x1F) << 6 | (utf8TokUs[1] & 0x3F));
            return 2;
        }
        else if (0xE0 == (utf8TokUs[0] & 0xF0))
        {
            //Tokensize: 3 bytes
            if ((0x80 != (utf8TokUs[1] & 0xC0))
                || (0x80 != (utf8TokUs[2] & 0xC0))
               )
            {
                return 0;
            }

            ucs4 = static_cast<const uint32_t>(
                      (utf8TokUs[0] & 0x0F) << 12
                    | (utf8TokUs[1] & 0x3F) << 6
                    | (utf8TokUs[2] & 0x3F)
                );
            return 3;
        }
        else if (0xF0 == (utf8TokUs[0] & 0xF8))
        {
            //Tokensize: 4 bytes
            if ((0x80 != (utf8TokUs[1] & 0xC0))
                || (0x80 != (utf8TokUs[2] & 0xC0))
                || (0x80 != (utf8TokUs[3] & 0xC0))
               )
            {
                return 0;
            }

            ucs4 = static_cast<const uint32_t>(
                      (utf8TokUs[0] & 0x7) << 18
                    | (utf8TokUs[1] & 0x3F) << 12
                    | (utf8TokUs[2] & 0x3F) << 6
                    | (utf8TokUs[3] & 0x3F)
                );
            return 4;
        }
        return 0;
    }
}