#pragma once
#include <string>
#include <SRefl/SRefl.hpp>

namespace crystal
{
	struct EngineInitArguments
	{
        // Core
		uint32_t	WindowWidth, WindowHeight;
		uint32_t	FPSLimit;
		bool		WindowResizable;
        std::string WindowTitle;
		bool		Enable4xMSAA;
		uint32_t	MSAAQuality;

        // Asset
        std::string                 BuiltinPackagePath;
        std::vector<std::string>    ExtraPackages;
	};
}

template<>
struct SRefl::TypeInfo<crystal::EngineInitArguments>
{
    SREFL_TYPEINFO_HEADER(crystal::EngineInitArguments);
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

            SREFL_FIELD(BuiltinPackagePath),
            SREFL_FIELD(ExtraPackages)
        );
    }
};