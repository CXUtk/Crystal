#pragma once
#include "../GraphicsCommon.h"
#include "IShaderResource.h"

namespace crystal
{
	class IRenderTarget2D : public IShaderResource
	{
	public:
		virtual ~IRenderTarget2D() = 0 {};
	};
}