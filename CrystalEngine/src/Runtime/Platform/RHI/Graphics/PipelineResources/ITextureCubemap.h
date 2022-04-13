#pragma once
#include "../GraphicsCommon.h"
#include "IShaderResource.h"

namespace crystal
{
    class ITextureCubemap : public IShaderResource
    {
    public:
        virtual ~ITextureCubemap() = 0 {};
    };
}