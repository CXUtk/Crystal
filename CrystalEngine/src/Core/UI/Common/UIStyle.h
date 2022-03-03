#pragma once
#include <Interfaces/Graphics/GraphicsCommon.h>
#include <Core/Render/RenderCommon.h>
#include "UICommon.h"

namespace crystal
{
    class UIStyle
    {
    public:
        UIStyle();
        ~UIStyle();

        Color4f GetPanelBorderColor() const;
        Color4f GetPanelInnerColor() const;
        std::shared_ptr<ITexture2D> GetPanelTexture() const;
    };
}