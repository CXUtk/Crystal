#pragma once
#include "../GraphicsCommon.h"
#include "IShaderResource.h"

namespace crystal
{
    class ITextureCubemap : public IShaderResource
    {
    public:
        virtual ~ITextureCubemap() = 0 {};
        virtual void ReplaceMipmap(std::shared_ptr<ITextureCubemap> mipCubemap, int mipLevel) = 0;
    };
}