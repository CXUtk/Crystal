#pragma once
#include "../GraphicsCommon.h"
namespace crystal
{
	class IShaderResource
	{
	public:
		virtual ~IShaderResource() = 0 {};
		virtual void GetShaderResourceHandle(void* pHandle) const = 0;
	};
}