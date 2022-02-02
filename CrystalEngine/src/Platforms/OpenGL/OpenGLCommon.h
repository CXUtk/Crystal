#pragma once
#include <Crystal.h>
#include <Core/Platform/Platforms.h>
#include <Core/Platform/Graphics/IGraphicsDevice.h>
#include <Core/Platform/Graphics/IShaders.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace crystal
{
	class OpenGLGraphicsDevice;

	void InitOpenGLCommons();

	const char* VertexElementFormatToShaderVarConvert(VertexElementFormat format);
	const char* SemanticNameConvert(SemanticType semanticType);
	const char* ShaderModelConvert(ShaderType shaderType);
	GLuint PrimitiveTypeToTopologyConvert(PrimitiveType type);
	ComponentFormat StringToComponentFormatConvert(const std::string& type);
	size_t ComponentFormatToSizeConvert(ComponentFormat format);
}