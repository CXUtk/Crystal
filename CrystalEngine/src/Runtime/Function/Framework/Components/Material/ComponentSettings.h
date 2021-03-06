#pragma once
#include <SRefl/SRefl.hpp>
#include <Core/Utils/SJsonHelper.h>
#include <Resource/Asset/AssetManager.h>

namespace crystal
{
    enum class MaterialType
    {
        Null,
        Lambertian,
        Glass,
        Mirror,
        Plastic,
        Metal
    };

    struct LambertianMaterialSettings
    {
        // Diffuse color
        URI Kd;
    };

    struct GlassMaterialSettings
    {
        // Specular reflection color
        URI Ks;
        // Specular transmission color
        URI Tf;
        // Index of refraction
        Float IOR;
    };

    struct MirrorMaterialSettings
    {
        URI Ks;
    };

    struct PlasticMaterialSettings
    {
        URI Kd;
        // Roughness texture of surface
        URI Roughness;
        // Index of refraction
        Float IOR;
    };


    struct MetalMaterialSettings
    {
        URI Kd;
        // Roughness texture of surface
        URI Roughness;
        // Index of refraction
        Spectrum IOR;
        // Index of refraction (Complex)
        Spectrum K;
    };
}

template<>
struct SRefl::TypeInfo<crystal::LambertianMaterialSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::LambertianMaterialSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Kd)
        );
    }
};

template<>
struct SRefl::TypeInfo<crystal::GlassMaterialSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::GlassMaterialSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Ks),
            SREFL_FIELD(Tf),
            SREFL_FIELD(IOR)
        );
    }
};

template<>
struct SRefl::TypeInfo<crystal::MirrorMaterialSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::MirrorMaterialSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Ks)
        );
    }
};

template<>
struct SRefl::TypeInfo<crystal::PlasticMaterialSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::PlasticMaterialSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Kd),
            SREFL_FIELD(Roughness),
            SREFL_FIELD(IOR)
        );
    }
};

template<>
struct SRefl::TypeInfo<crystal::MetalMaterialSettings>
{
    SREFL_TYPEINFO_HEADER(crystal::MetalMaterialSettings);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Kd),
            SREFL_FIELD(Roughness),
            SREFL_FIELD(IOR),
            SREFL_FIELD(K)
        );
    }
};

template<>
struct SRefl::EnumInfo<crystal::MaterialType>
{
    SREFL_TYPEINFO_HEADER(crystal::MaterialType);
    constexpr static auto _ENUMLIST()
    {
        return std::make_tuple(
            SREFL_ENUM_TERM(Null),
            SREFL_ENUM_TERM(Lambertian),
            SREFL_ENUM_TERM(Glass),
            SREFL_ENUM_TERM(Mirror),
            SREFL_ENUM_TERM(Plastic),
            SREFL_ENUM_TERM(Metal)
        );
    }
#define LISTFUNC(F) F(Null) F(Lambertian) F(Glass) F(Mirror) F(Plastic) F(Metal)
    GENERATE_ENUM_MAPPING(crystal::MaterialType, LISTFUNC)
#undef LISTFUNC
};