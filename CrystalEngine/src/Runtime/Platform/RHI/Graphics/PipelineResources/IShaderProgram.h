#pragma once
#include "../GraphicsCommon.h"

namespace crystal
{
	enum ShaderMask : int
	{
		CRYSTAL_SHADERMASK_NO_SHADER = 0,
		CRYSTAL_SHADERMASK_VERTEX_SHADER = 1 << 0,
		CRYSTAL_SHADERMASK_FRAGMENT_SHADER = 1 << 1,
		CRYSTAL_SHADERMASK_GEOMETRY_SHADER = 1 << 2,
		CRYSTAL_SHADERMASK_COMPUTE_SHADER = 1 << 3
	};

	inline ShaderMask operator&(ShaderMask a, ShaderMask b)
	{
		return static_cast<ShaderMask>(static_cast<int>(a) & static_cast<int>(b));
	}
	inline ShaderMask operator|(ShaderMask a, ShaderMask b)
	{
		return static_cast<ShaderMask>(static_cast<int>(a) | static_cast<int>(b));
	}

	class IShaderProgram
	{
	public:
		virtual ~IShaderProgram() = 0 {};

		virtual void SetUniform1f(const std::string& name, float value) = 0;
        virtual void SetUniformVec3f(const std::string& name, const Vector3f& value) = 0;
        virtual void SetUniformMat3f(const std::string& name, const Matrix3f& value) = 0;
		virtual void SetUniformMat4f(const std::string& name, const Matrix4f& value) = 0;
		virtual ShaderMask GetShaderMask() const = 0;
	};
}