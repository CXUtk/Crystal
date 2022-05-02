#pragma once
#include <SRefl/SRefl.hpp>
#include <Core/Utils/SJsonHelper.h>

namespace crystal
{
    enum class ShapeType
    {
        Sphere,
        Triangle
    };

    struct SphereSettings
    {
        Float Radius;
    };
}

template<>
struct SRefl::TypeInfo<crystal::SphereSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::SphereSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Radius)
        );
    }
};

template<>
struct SRefl::EnumInfo<crystal::ShapeType>
{
    SREFL_TYPEINFO_HEADER(crystal::ShapeType);
    constexpr static auto _ENUMLIST()
    {
        return std::make_tuple(
            SREFL_ENUM_TERM(Sphere),
            SREFL_ENUM_TERM(Triangle)
        );
    }
#define LISTFUNC(F) F(Sphere) F(Triangle)
    GENERATE_ENUM_MAPPING(crystal::ShapeType, LISTFUNC)
#undef LISTFUNC
};