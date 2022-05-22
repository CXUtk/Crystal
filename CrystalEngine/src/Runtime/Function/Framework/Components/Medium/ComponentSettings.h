#pragma once
#include <SRefl/SRefl.hpp>
#include <Core/Utils/SJsonHelper.h>

namespace crystal
{
    enum class MediumType
    {
        Homogeneous,
        Grid
    };

    struct HomogeneousMediumSettings
    {
        Float Sigma_A;
        Float Sigma_S;
        Float G;
    };

    struct GridMediumSettings
    {
        Float Sigma_A;
        Float Sigma_S;
        Float G;
    };
}

template<>
struct SRefl::TypeInfo<crystal::HomogeneousMediumSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::HomogeneousMediumSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Sigma_A),
            SREFL_FIELD(Sigma_S),
            SREFL_FIELD(G)
        );
    }
};

template<>
struct SRefl::TypeInfo<crystal::GridMediumSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::GridMediumSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Sigma_A),
            SREFL_FIELD(Sigma_S),
            SREFL_FIELD(G)
        );
    }
};

template<>
struct SRefl::EnumInfo<crystal::MediumType>
{
    SREFL_TYPEINFO_HEADER(crystal::MediumType);
    constexpr static auto _ENUMLIST()
    {
        return std::make_tuple(
            SREFL_ENUM_TERM(Homogeneous),
            SREFL_ENUM_TERM(Grid)
        );
    }
#define LISTFUNC(F) F(Homogeneous) F(Grid)
    GENERATE_ENUM_MAPPING(crystal::MediumType, LISTFUNC)
#undef LISTFUNC
};