#pragma once
#include <string>
#include <SRefl/SRefl.hpp>
#include <Platform/RHI/Graphics/GraphicsCommon.h>

namespace crystal
{
    struct Texture2DMetaInfo
    {
        std::string     Target;
        RenderFormat    Format;
        size_t          MipmapLevels;
    };

    struct CPUTexture2DMetaInfo
    {
        std::string     Target;

        DataFormat      TargetComponentFormat;
        size_t          TargetComponents;

        DataFormat      InternalComponentFormat;
        size_t          InternalComponents;

        bool            IsHDR;
        size_t          MipmapLevels;
    };


    struct TextureCubemapMetaInfo
    {
        std::string     PosX;
        std::string     NegX;
        std::string     PosY;
        std::string     NegY;
        std::string     PosZ;
        std::string     NegZ;

        RenderFormat    Format;
        size_t          MipmapLevels;
    };

    struct CPUTextureCubemapMetaInfo
    {
        std::string     PosX;
        std::string     NegX;
        std::string     PosY;
        std::string     NegY;
        std::string     PosZ;
        std::string     NegZ;

        DataFormat      TargetComponentFormat;
        size_t          TargetComponents;

        DataFormat      InternalComponentFormat;
        size_t          InternalComponents;

        bool            IsHDR;
        size_t          MipmapLevels;
    };

    struct TextureCubemapMetaWithMipsInfo
    {
        std::string     PosXPattern;
        std::string     NegXPattern;
        std::string     PosYPattern;
        std::string     NegYPattern;
        std::string     PosZPattern;
        std::string     NegZPattern;

        RenderFormat    Format;
        size_t          MipmapLevels;
    };
}


template<>
struct SRefl::TypeInfo<Texture2DMetaInfo>
{
    SREFL_TYPEINFO_HEADER(Texture2DMetaInfo);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Target),
            SREFL_FIELD(Format),
            SREFL_FIELD(MipmapLevels)
        );
    }
};

template<>
struct SRefl::TypeInfo<TextureCubemapMetaInfo>
{
    SREFL_TYPEINFO_HEADER(TextureCubemapMetaInfo);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(PosX),
            SREFL_FIELD(NegX),
            SREFL_FIELD(PosY),
            SREFL_FIELD(NegY),
            SREFL_FIELD(PosZ),
            SREFL_FIELD(NegZ),
            SREFL_FIELD(Format),
            SREFL_FIELD(MipmapLevels)
        );
    }
};


template<>
struct SRefl::TypeInfo<TextureCubemapMetaWithMipsInfo>
{
    SREFL_TYPEINFO_HEADER(TextureCubemapMetaWithMipsInfo);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(PosXPattern),
            SREFL_FIELD(NegXPattern),
            SREFL_FIELD(PosYPattern),
            SREFL_FIELD(NegYPattern),
            SREFL_FIELD(PosZPattern),
            SREFL_FIELD(NegZPattern),
            SREFL_FIELD(Format),
            SREFL_FIELD(MipmapLevels)
        );
    }
};

template<>
struct SRefl::TypeInfo<CPUTexture2DMetaInfo>
{
    SREFL_TYPEINFO_HEADER(CPUTexture2DMetaInfo);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(Target),
            SREFL_FIELD(TargetComponentFormat),
            SREFL_FIELD(TargetComponents),
            SREFL_FIELD(InternalComponentFormat),
            SREFL_FIELD(InternalComponents),
            SREFL_FIELD(IsHDR),
            SREFL_FIELD(MipmapLevels)
        );
    }
};


template<>
struct SRefl::TypeInfo<CPUTextureCubemapMetaInfo>
{
    SREFL_TYPEINFO_HEADER(CPUTextureCubemapMetaInfo);
    constexpr static auto _FIELDLIST()
    {
        return std::make_tuple(
            SREFL_FIELD(PosX),
            SREFL_FIELD(NegX),
            SREFL_FIELD(PosY),
            SREFL_FIELD(NegY),
            SREFL_FIELD(PosZ),
            SREFL_FIELD(NegZ),
            SREFL_FIELD(TargetComponentFormat),
            SREFL_FIELD(TargetComponents),
            SREFL_FIELD(InternalComponentFormat),
            SREFL_FIELD(InternalComponents),
            SREFL_FIELD(IsHDR),
            SREFL_FIELD(MipmapLevels)
        );
    }
};