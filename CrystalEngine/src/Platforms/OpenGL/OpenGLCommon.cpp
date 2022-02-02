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

	Matrix4f crystal_look_at(const Point3f& eye, const Point3f& center, const Vector3f& up)
	{
		return glm::lookAtRH(eye, center, up);
	}
	Matrix4f crystal_perspective(Float fovY, Float aspectRatio, Float zNear, Float zFar)
	{
		return glm::perspectiveRH_NO(fovY, aspectRatio, zNear, zFar);
	}

	void InitOpenGLCommons()
	{

	}

	GLuint PrimitiveTypeToTopologyConvert(PrimitiveType type)
	{
		return DrawPrimitivesTypeMapper[(int)type];
	}
}