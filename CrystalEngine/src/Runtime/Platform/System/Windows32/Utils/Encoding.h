#pragma once
#include <string>

class Win32Encoding
{
public:
    static std::string WStringToUtf8String(const std::wstring& wstr);
    static std::wstring Utf8StringToWString(const std::string& str);
};