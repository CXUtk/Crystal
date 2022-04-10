#pragma once
#include "OpenGLCommon.h"

namespace crystal
{
	struct GLUniformVariable
	{
		GLint			Location;
		size_t			Offset;
		ComponentFormat Format;
	};

	class OpenGLShaderProgram : public IShaderProgram
	{
	public:
		OpenGLShaderProgram(GraphicsDevice* graphicsDevice, std::shared_ptr<VertexShader> vertexShader,
			std::shared_ptr<FragmentShader> fragmentShader, const UniformVariableCollection& uniforms);
		~OpenGLShaderProgram() override;

		GLuint GetHandle() const { return m_shaderHandle; }

		virtual void Apply() override;
		virtual void SetUniform1f(const std::string& name, float value) override;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) override;
	private:
		GraphicsDevice*		m_pGraphicsDevice = nullptr;
		GLuint				m_shaderHandle{};

		std::map<std::string, size_t>	m_uniformOffsetTable{};
		std::unique_ptr<char[]>			m_pConstantBufferData{};
		std::vector<GLUniformVariable>	m_uniformVariables{};

		bool	m_constBufferDirty = true;
		size_t	m_constBufferSize = 0;

		void m_SetOneUniform(const GLUniformVariable& var);
	};

}