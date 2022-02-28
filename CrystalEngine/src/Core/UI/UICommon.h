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

    namespace Flags
    {
        enum PropagationFlags : uint32_t
        {
            None = 0,
            MouseClick = 1 << 0,
            MouseUp = 1 << 1,
            MouseDown = 1 << 2,

            All = -1
        };

        inline PropagationFlags operator|(PropagationFlags a, PropagationFlags b)
        {
            return static_cast<PropagationFlags>(static_cast<int>(a) | static_cast<int>(b));
        }

        inline PropagationFlags operator&(PropagationFlags a, PropagationFlags b)
        {
            return static_cast<PropagationFlags>(static_cast<int>(a) & static_cast<int>(b));
        }
    }

    using PropagationFlags = Flags::PropagationFlags;

}