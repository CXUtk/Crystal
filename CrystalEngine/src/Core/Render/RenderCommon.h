#pragma once
#include <Interfaces/Interfaces.h>
#include <Core/Utils/Geometry.h>

namespace crystal
{
    class SpriteBatch;
    class GeometryRenderer;

    struct RenderPayload
    {
        SpriteBatch*                            SpriteBatch = nullptr;
        std::shared_ptr<IPipelineStateObject>   PSO = nullptr;
    };

    BIT_FLAGS_BEGIN(SpriteEffect, uint32_t)
    {
        None = 0,
        FlipHorizontal = 1,
        FlipVertical = 1 << 1,
    };
    BIT_FLAGS_END(SpriteEffect);

    enum class SpriteSortMode
    {
        Deferred,
        Immediate,
        Texture,
        FrontToBack,
        BackToFront
    };

    /**
     * @brief Describe the slicing info of given texture 2d
    */
    struct SliceInfo
    {
        int Top;
        int Left;
        int Bot;
        int Right;

        int DrawFlags;
    };

    constexpr int Slice_Nine = 0x1FF;
    constexpr int Slice_Six = 0x1F8;
}