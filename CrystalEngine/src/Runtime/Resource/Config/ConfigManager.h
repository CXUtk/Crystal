#pragma once
#include "AppInitArguments.h"

namespace crystal
{
    class ConfigManager
    {
    public:
        ConfigManager();
        ~ConfigManager();

        void LoadConfigs();
        const AppInitArguments& GetAppInitArgs() const { return m_appInitArgs; }

    private:
        AppInitArguments m_appInitArgs;
    };
}