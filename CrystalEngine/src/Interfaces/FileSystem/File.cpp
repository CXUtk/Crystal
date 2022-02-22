#include "File.h"
#include <fstream>
#include <sstream>
#include <Core/Utils/Misc.h>

namespace crystal
{
	std::string File::ReadAllText(const path_type& path)
	{
        std::ifstream iFile;
        iFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::string str;
        try
        {
            iFile.open(path);
            std::stringstream strStream;
            strStream << iFile.rdbuf();
            iFile.close();
            str = strStream.str();
        }
        catch (std::ifstream::failure e)
        {
            throw std::exception(string_format("Failed to read file %s: %s", path.c_str(), e.what()).c_str());
        }
        return str;
	}
}
