#include "OpenGLCommon.h"
#include <array>

namespace crystal
{
	template<size_t N>
	constexpr std::array<GLuint, N> GeneratePrimititveToModeMapping()
	{
		std::array<GLuint, N> M{};
		M[(int)PrimitiveType::POINTS] = GL_POINTS;
		M[(int)PrimitiveType::LINE_LIST] = GL_LINES;
		M[(int)PrimitiveType::LINE_STRIP] = GL_LINE_STRIP;
		M[(int)PrimitiveType::TRIANGLE_LIST] = GL_TRIANGLES;
		M[(int)PrimitiveType::TRIANGLE_STRIP] = GL_TRIANGLE_STRIP;
		return M;
	}
	static constexpr auto DrawPrimitivesTypeMapper = GeneratePrimititveToModeMapping<(size_t)PrimitiveType::__COUNT>();

	template<size_t N>
	constexpr std::array<GLuint, N> GenerateBufferUsageToOpenGLMapping()
	{
		std::array<GLuint, N> M{};
		M[(int)BufferUsage::Default] = GL_DYNAMIC_DRAW;
		M[(int)BufferUsage::Immutable] = GL_STATIC_DRAW;
		M[(int)BufferUsage::CPURead] = GL_DYNAMIC_DRAW;
		M[(int)BufferUsage::CPUWrite] = GL_DYNAMIC_DRAW;
		M[(int)BufferUsage::CPURWrite] = GL_DYNAMIC_DRAW;
		return M;
	}
	static constexpr auto BufferUsageToOpenGLMapper = GenerateBufferUsageToOpenGLMapping<(size_t)BufferUsage::__COUNT>();

	template<size_t N>
	constexpr std::array<GLenum, N> GenerateDataFormatMapping()
	{
		std::array<GLenum, N> M{};
		M[(int)DataFormat::Byte8] = GL_UNSIGNED_BYTE;
		M[(int)DataFormat::SByte8] = GL_BYTE;
		M[(int)DataFormat::UShort16] = GL_UNSIGNED_SHORT;
		M[(int)DataFormat::Short16] = GL_SHORT;
		M[(int)DataFormat::UInt32] = GL_UNSIGNED_INT;
		M[(int)DataFormat::Int32] = GL_INT;
		M[(int)DataFormat::Float32] = GL_FLOAT;
		return M;
	}
	constexpr auto DataFormatToDataTypeMapping = GenerateDataFormatMapping<(size_t)DataFormat::__COUNT>();

	template<size_t N>
	constexpr std::array<GLenum, N> GenerateShaderTypeMapping()
	{
		std::array<GLenum, N> M{};
		M[(int)ShaderType::VERTEX_SHADER] = GL_VERTEX_SHADER;
		M[(int)ShaderType::FRAGMENT_SHADER] = GL_FRAGMENT_SHADER;
		M[(int)ShaderType::GEOMETRY_SHADER] = GL_GEOMETRY_SHADER;
		M[(int)ShaderType::COMPUTE_SHADER] = GL_COMPUTE_SHADER;
		return M;
	}
	constexpr auto ShaderTypeToOpenGLMapping = GenerateShaderTypeMapping<(size_t)ShaderType::__COUNT>();


	Matrix4f crystal_look_at(const Point3f& eye, const Point3f& center, const Vector3f& up)
	{
		return glm::lookAtLH(eye, center, up);
	}
	Matrix4f crystal_perspective(Float fovY, Float aspectRatio, Float zNear, Float zFar)
	{
		return glm::perspectiveLH_NO(fovY, aspectRatio, zNear, zFar);
	}


	GLenum ShaderTypeToModelConvert(ShaderType shaderType)
	{
		return ShaderTypeToOpenGLMapping[(int)shaderType];
	}

	GLenum DataFormatConvert(DataFormat format)
	{
		return DataFormatToDataTypeMapping[(int)format];
	}

	GLuint BufferUsageToOpenGLConvert(BufferUsage usage)
	{
		return BufferUsageToOpenGLMapper[(int)usage];
	}

	GLuint PrimitiveTypeToTopologyConvert(PrimitiveType type)
	{
		return DrawPrimitivesTypeMapper[(int)type];
	}


	void InitOpenGLCommons()
	{
		static bool initialized = false;
		if (initialized) return;
		{
		}
		initialized = true;
	}
}