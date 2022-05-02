#pragma once
#include <SRefl/SRefl.hpp>
#include <Core/Math/Geometry.h>
#include <Core/Utils/SJsonHelper.h>

namespace crystal
{
    enum class CameraType
    {
        Perspective
    };

    struct PerspectiveCameraSetting
    {
        Float FovY;
        Float Aspect;
        Float ZNear;
        Float ZFar;
    };
}

template<>
struct SRefl::TypeInfo<crystal::PerspectiveCameraSetting>
{
    SREFL_TYPEINFO_HEADER(crystal::PerspectiveCameraSetting);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(FovY),
            SREFL_FIELD(Aspect),
            SREFL_FIELD(ZNear),
            SREFL_FIELD(ZFar)
        );
    }
};

template<>
struct SRefl::EnumInfo<crystal::CameraType>
{
    SREFL_TYPEINFO_HEADER(crystal::CameraType);
    constexpr static auto _ENUMLIST()
    {
        return std::make_tuple(
            SREFL_ENUM_TERM(Perspective)
        );
    }
#define LISTFUNC(F) F(Perspective)
    GENERATE_ENUM_MAPPING(crystal::CameraType, LISTFUNC)
#undef LISTFUNC
};