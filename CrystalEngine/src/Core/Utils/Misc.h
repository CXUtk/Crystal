#pragma once
#include <string>

/**
 * @brief Get the formatted std::string
 * @tparam ...Args
 * @param format The same format as printf
 * @param ...args The same arguments as arguments of printf
 * @return Formatted string
*/
template<typename ... Args>
inline std::string string_format(const std::string& format, Args&& ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::exception("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

inline std::string GetNameByIndex(int index)
{
    std::string name;
    while (index)
    {
        name.push_back('a' + (index % 26));
        index /= 26;
    }
    return name;
}