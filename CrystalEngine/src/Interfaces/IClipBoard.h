#pragma once
#include <string>
#include <fstream>

namespace crystal
{
    class IClipBoard
    {
    public:
        virtual ~IClipBoard() = 0 {};

        virtual void Copy(const std::u32string& text) = 0;
        virtual std::u32string Paste() = 0;
    };
}