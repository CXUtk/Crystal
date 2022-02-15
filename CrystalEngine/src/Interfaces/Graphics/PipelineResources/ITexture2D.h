#pragma once
#include "../GraphicsCommon.h"
#include "IShaderResource.h"

namespace crystal
{
	class ITexture2D : public IShaderResource
	{
	public:
		virtual ~ITexture2D() = 0 {};
	};
}