#pragma once
#include <SRefl/SRefl.hpp>
#include <Core/Utils/SJsonHelper.h>

namespace crystal
{
    enum class LightType
    {
        PointLight,
        DiffuseAreaLight
    };

    struct PointLightSettings
    {
        Spectrum Intensity;
    };

    struct DiffuseAreaLightSettings
    {
        Spectrum    LEmit;
        int         NumSamples;
    };
}

template<>
struct SRefl::TypeInfo<crystal::PointLightSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::PointLightSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Intensity)
        );
    }
};

template<>
struct SRefl::TypeInfo<crystal::DiffuseAreaLightSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::DiffuseAreaLightSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(LEmit)
        );
    }
};

template<>
struct SRefl::EnumInfo<crystal::LightType>
{
    SREFL_TYPEINFO_HEADER(crystal::LightType);
    constexpr static auto _ENUMLIST()
    {
        return std::make_tuple(
            SREFL_ENUM_TERM(PointLight),
            SREFL_ENUM_TERM(DiffuseAreaLight)
        );
    }
#define LISTFUNC(F) F(PointLight) F(DiffuseAreaLight)
    GENERATE_ENUM_MAPPING(crystal::LightType, LISTFUNC)
#undef LISTFUNC
};