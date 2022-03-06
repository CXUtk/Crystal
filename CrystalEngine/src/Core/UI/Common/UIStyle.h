#pragma once
#include <Interfaces/Graphics/GraphicsCommon.h>
#include <Core/Render/RenderCommon.h>
#include "UICommon.h"

namespace crystal
{
    class UIStyle
    {
    public:

        static Color4f GetPanelBorderColor() { return Color4f(0.55f, 0.55f, 0.55f, 1.f); }
        static Color4f GetPanelInnerColor() { return Color4f(0.17f, 0.17f, 0.17f, 1.f); }
        static Color4f GetButtonColor() { return Color4f(0.17f, 0.3f, 0.5f, 1.f); }
        static Color4f GetButtonHoverColor() { return Color4f(0.2f, 0.41f, 0.67f, 1.f); }
        static Color4f GetProgressBarBackgroundColor() { return Color4f(0.17f, 0.17f, 0.17f, 1.f); }
    };
}