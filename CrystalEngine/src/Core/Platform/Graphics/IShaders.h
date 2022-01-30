#pragma once
#include "GraphicsCommon.h"

namespace crystal
{
	class IShader
	{
	public:
		virtual ~IShader() = 0 {};

		virtual ShaderType GetShaderType() const = 0;
		virtual void Apply() = 0;
	};


	class IVertexShader : public IShader
	{
	public:
		virtual ~IVertexShader() = 0 {};
	};

	class IFragmentShader : public IShader
	{
	public:
		virtual ~IFragmentShader() = 0 {};
	};
}