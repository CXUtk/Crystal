#pragma once
#include "../GraphicsCommon.h"
#include "IShaderResource.h"

namespace crystal
{
	class ITexture2D : public IShaderResource
	{
	public:
		virtual ~ITexture2D() = 0 {};
		virtual Vector2i GetSize() const = 0;

        virtual void ReplaceContent(void* data, size_t size, int level) = 0;
	};
}