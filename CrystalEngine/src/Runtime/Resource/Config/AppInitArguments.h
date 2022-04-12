#pragma once
#include <string>
#include <SRefl/SRefl.hpp>

namespace crystal
{
	struct AppInitArguments
	{
        // Core
		uint32_t	WindowWidth, WindowHeight;
		uint32_t	FPSLimit;
		bool		WindowResizable;
        std::string WindowTitle;
		bool		Enable4xMSAA;
		uint32_t	MSAAQuality;

        // Asset
        std::string                 BuiltinPackage;
        std::vector<std::string>    ExtraPackages;
	};
}

template<>
struct SRefl::TypeInfo<crystal::AppInitArguments>
{
    SREFL_TYPEINFO_HEADER(crystal::AppInitArguments);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(WindowWidth),
            SREFL_FIELD(WindowHeight),
            SREFL_FIELD(FPSLimit),
            SREFL_FIELD(WindowResizable),
            SREFL_FIELD(WindowTitle),
            SREFL_FIELD(Enable4xMSAA),
            SREFL_FIELD(MSAAQuality),

            SREFL_FIELD(BuiltinPackage),
            SREFL_FIELD(ExtraPackages)
        );
    }
};