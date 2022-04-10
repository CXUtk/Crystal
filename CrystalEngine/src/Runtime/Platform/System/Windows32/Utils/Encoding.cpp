#include "Encoding.h"
#include <Windows.h>

std::string Win32Encoding::WStringToUtf8String(const std::wstring& wstr)
{
    int len;
    int slength = (int)wstr.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, 0, 0, 0, 0);
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, buf, len, 0, 0);
    std::string r(buf);
    delete[] buf;
    return r;
}

std::wstring Win32Encoding::Utf8StringToWString(const std::string& str)
{
    int len;
    int slength = (int)str.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
