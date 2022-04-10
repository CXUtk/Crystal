#pragma once
#include <Crystal.h>
#include <Core/Platform/Platforms.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace crystal
{
	class OpenGLGraphicsDevice;

	void InitOpenGLCommons();

	GLenum ShaderTypeToModelConvert(ShaderType shaderType);
	GLenum DataFormatConvert(DataFormat format);
	GLuint BufferUsageToOpenGLConvert(BufferUsage usage);
	GLuint PrimitiveTypeToTopologyConvert(PrimitiveType type);
}