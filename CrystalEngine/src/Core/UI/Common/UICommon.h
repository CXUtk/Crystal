#pragma once
#include <Interfaces/Graphics/GraphicsCommon.h>
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

    struct Layout1D
    {
        Float Absolute;
        Float Relative;
    };

    struct SizeLayout
    {
        Layout1D Width;
        Layout1D Height;
    };

    enum class OverflowStyle
    {
        Overflow,
        Hidden,
    };

    BIT_FLAGS_BEGIN(PropagationFlags, uint32_t)
    {
        None = 0,
        MouseClick = 1 << 0,
        MouseUp = 1 << 1,
        MouseDown = 1 << 2,

        All = -1
    };
    BIT_FLAGS_END(PropagationFlags);

}