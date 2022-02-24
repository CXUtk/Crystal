#include "File.h"
#include <fstream>
#include <sstream>
#include <Core/Utils/Misc.h>
#include <functional>

namespace crystal
{
	std::string File::ReadAllText(const path_type& path)
	{
        std::ifstream iStream;
        iStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::string str;
        try
        {
            iStream.open(path);
            if (iStream.is_open())
            {
                std::stringstream strStream;
                strStream << iStream.rdbuf();
                str = strStream.str();
                iStream.close();
            }
            else
            {
                throw std::exception("Cannot open file");
            }
        }
        catch (std::exception& e)
        {
            throw std::exception(string_format("Failed to read file %s: %s", path.string().c_str(),
                e.what()).c_str());
        }
        return str;
	}

    void File::WriteAllText(const path_type& path, const std::string& text)
    {
        std::ofstream oStream;
        try
        {
            oStream.open(path);
            if (oStream.is_open())
            {
                oStream << text;
                oStream.close();
            }
            else
            {
                throw std::exception("Cannot open file");
            }
        }
        catch (std::exception& e)
        {
            throw std::exception(string_format("Failed to write to file %s: %s", path.string().c_str(), e.what()).c_str());
        }
    }

    std::vector<std::string> File::ReadAllLines(const path_type& path)
    {
        std::vector<std::string> lines;
        std::ifstream iStream;
        iStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            iStream.open(path);
            if (iStream.is_open())
            {
                std::string line;
                while (std::getline(iStream, line))
                {
                    lines.push_back(line);
                }
                iStream.close();
            }
            else
            {
                throw std::exception("Cannot open file");
            }
        }
        catch (std::exception& e)
        {
            throw std::exception(string_format("Failed to read file %s: %s", path.string().c_str(), e.what()).c_str());
        }
        return lines;
    }

    void File::WriteAllLines(const path_type& path, const std::vector<std::string>& lines)
    {
        std::ofstream oStream;
        try
        {
            oStream.open(path);
            if (oStream.is_open())
            {
                for (auto& line : lines)
                {
                    oStream << line << '\n';
                }
                oStream.close();
            }
            else
            {
                throw std::exception("Cannot open file");
            }
        }
        catch (std::exception& e)
        {
            throw std::exception(string_format("Failed to write to file %s: %s", path.string().c_str(), e.what()).c_str());
        }
    }

    std::vector<Byte> File::ReadAllBytes(const path_type& path)
    {
        std::vector<Byte> buffer;
        std::ifstream iStream;
        iStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            iStream.open(path);
            if (iStream.is_open())
            {
                //get length of file
                iStream.seekg(0, std::ios::end);
                size_t length = iStream.tellg();
                iStream.seekg(0, std::ios::beg);

                buffer.reserve(length);
                std::copy(std::istreambuf_iterator<char>(iStream),
                           std::istreambuf_iterator<char>(),
                           std::back_inserter(buffer));
                iStream.close();
            }
            else
            {
                throw std::exception("Cannot open file");
            }
        }
        catch (std::exception& e)
        {
            throw std::exception(string_format("Failed to read file %s: %s", path.string().c_str(), e.what()).c_str());
        }
        return buffer;
    }

    void File::WriteAllBytes(const path_type& path, const void* src, size_t size)
    {
        std::ofstream oStream;
        try
        {
            oStream.open(path);
            if (oStream.is_open())
            {
                oStream.write((const char*)src, size);
                oStream.close();
            }
            else
            {
                throw std::exception("Cannot open file");
            }
        }
        catch (std::exception& e)
        {
            throw std::exception(string_format("Failed to write to file %s: %s", path.string().c_str(), e.what()).c_str());
        }
    }

    bool File::Exists(const path_type& path)
    {
        auto status = fs::status(path);
        return fs::exists(status) && fs::is_regular_file(status);
    }

    void File::Delete(const path_type& path)
    {
        auto status = fs::status(path);
        if (!fs::is_regular_file(status))
        {
            throw std::invalid_argument("Given path is not a regular file");
        }
        fs::remove(path);
    }

    void File::Copy(const path_type& srcFile, const path_type& destFile, CopyOptions options)
    {
        fs::copy_options option = fs::copy_options::none;
        switch (options )
        {
        case crystal::CopyOptions::None:
        {
            break;
        }
        case crystal::CopyOptions::Overwrite:
        {
            option = fs::copy_options::overwrite_existing;
        }
        break;
        case crystal::CopyOptions::SkipExisting:
        {
            option = fs::copy_options::skip_existing;
        }
        break;
        case crystal::CopyOptions::Update:
        {
            option = fs::copy_options::update_existing;
        }
        break;
        default:
            throw std::invalid_argument("Unknown CopyOptions");
            break;
        }
        fs::copy_file(srcFile, destFile);
    }

    void File::Move(const path_type& srcFile, const path_type& destFile)
    {
        fs::copy_file(srcFile, destFile);
        fs::remove(srcFile);
    }


    time_stamp File::GetLastAccessTime(const path_type& path)
    {
        auto status = fs::status(path);
        if (!fs::is_regular_file(status))
        {
            throw std::invalid_argument("Given path is not a regular file");
        }
        return fs::last_write_time(path);
    }
}
