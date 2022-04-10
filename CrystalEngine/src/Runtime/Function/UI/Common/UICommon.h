#pragma once
#include "Platform/RHI/Graphics/GraphicsCommon.h"
#include "Function/Render/RenderCommon.h"
#include <string>
#include <chrono>
#include <memory>
#include <fstream>
#include <vector>

namespace crystal
{
    constexpr bool EnableDebugDraw = true;

    class UIState;
    class UIStateMachine;

    class UIElement;
    class UIIconButton;
    class UIPanel;
    class UIWidget;

    struct Layout1D
    {
        Float Absolute;
        Float Relative;
        bool operator==(const Layout1D& other) const
        {
            return Absolute == other.Absolute && Relative == other.Relative;
        }
        bool operator!=(const Layout1D& other) const
        {
            return !(*this == other);
        }
    };

    struct SizeLayout
    {
        Layout1D Width;
        Layout1D Height;

        SizeLayout() = default;
        SizeLayout(int width, int height)
        {
            Width.Absolute = width;
            Width.Relative = 0;

            Height.Absolute = height;
            Height.Relative = 0;
        }

        SizeLayout(int width, float widthRel, int height, float heightRel)
        {
            Width.Absolute = width;
            Width.Relative = widthRel;

            Height.Absolute = height;
            Height.Relative = heightRel;
        }
    };

    enum class ImageType
    {
        Full,
        Sliced
    };

    enum class OverflowStyle
    {
        Overflow,
        Hidden,
    };

    BIT_FLAGS_BEGIN(PropagationFlags, uint32_t)
    {
        None = 0,
            MouseJustDown = 1 << 0,
            MouseJustUp = 1 << 1,
            MouseClicked = 1 << 2,
            MouseDoubleClicked = 1 << 3,
            MouseEnter = 1 << 4,
            MouseLeave = 1 << 5,
            MouseScroll = 1 << 6,
            Focus = 1 << 7,
            UnFocus = 1 << 8,
            All = (unsigned)-1
    };
    BIT_FLAGS_END(PropagationFlags);

    constexpr PropagationFlags MouseClickEvents = PropagationFlags::MouseJustDown | PropagationFlags::MouseJustUp
        | PropagationFlags::MouseClicked | PropagationFlags::MouseDoubleClicked;

    BIT_FLAGS_BEGIN(RecalculateMask, uint32_t)
    {
        None = 0,
        Mask_Width = 1 << 0,
        Mask_Height = 1 << 1,
        Mask_Pos = 1 << 2
    };
    BIT_FLAGS_END(RecalculateMask);
}