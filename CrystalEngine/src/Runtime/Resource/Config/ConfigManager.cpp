#include "ConfigManager.h"

#include <SJson/SJson.hpp>
#include "Platform/RHI/FileSystem/File.h"

namespace crystal
{
    ConfigManager::ConfigManager()
    {}
    ConfigManager::~ConfigManager()
    {}

    void ConfigManager::LoadConfigs()
    {
        m_appInitArgs = SJson::JsonConvert::Deserialize<AppInitArguments>(File::ReadAllText("assets/init.json"));
    }
}
