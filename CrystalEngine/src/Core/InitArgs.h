#pragma once
#include <string>
#include <SRefl/SRefl.hpp>

namespace crystal
{
	struct InitArgs
	{
		uint32_t	WindowWidth, WindowHeight;
		uint32_t	FPSLimit;
		bool		WindowResizable;
        std::string WindowTitle;
		bool		Enable4xMSAA;
		uint32_t	MSAAQuality;
	};
}

template<>
struct SRefl::TypeInfo<crystal::InitArgs>
{
    SREFL_TYPEINFO_HEADER(crystal::InitArgs);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(WindowWidth),
            SREFL_FIELD(WindowHeight),
            SREFL_FIELD(FPSLimit),
            SREFL_FIELD(WindowResizable),
            SREFL_FIELD(WindowTitle),
            SREFL_FIELD(Enable4xMSAA),
            SREFL_FIELD(MSAAQuality)
        );
    }
};