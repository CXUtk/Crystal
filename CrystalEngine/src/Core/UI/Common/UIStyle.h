#pragma once
#include <Interfaces/Graphics/GraphicsCommon.h>
#include <Core/Render/RenderCommon.h>
#include "UICommon.h"

namespace crystal
{
    class UIStyle
    {
    public:

        static Color4f GetPanelBorderColor() { return Color4f(0.329f, 0.329f, 0.36f, 1.f); }
        static Color4f GetPanelBorderColorHighlight() { return Color4f(0.415f, 0.415f, 0.458f, 1.f); }
        static Color4f GetPanelInnerColor() { return Color4f(0.176f, 0.176f, 0.188f, 1.f); }
        static Color4f GetContainerInnerColor() { return Color4f(0.145f, 0.145f, 0.149f, 1.f); }
        static Color4f GetButtonColor() { return Color4f(0.247f, 0.247f, 0.274f, 1.f); }
        static Color4f GetButtonHoverColor() { return Color4f(0.329f, 0.329f, 0.36f, 1.f); }
        static Color4f GetProgressBarBackgroundColor() { return Color4f(0.17f, 0.17f, 0.17f, 1.f); }
        static Color4f GetSliderCenterColor() { return Color4f(0.243f, 0.243f, 0.258f, 1.f); }
        static Color4f GetScrollBarColor() { return Color4f(0.407f, 0.407f, 0.407f, 1.f); }
        static Color4f GetScrollBarHoverColor() { return Color4f(0.619f, 0.619f, 0.619f, 1.f); }
        static Color4f GetWindowLegendColor() { return Color4f(0.f, 0.478f, 0.8f, 1.f); }
    };
}