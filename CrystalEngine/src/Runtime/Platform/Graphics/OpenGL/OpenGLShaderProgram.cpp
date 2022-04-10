#include "OpenGLShaderProgram.h"
#include "OpenGLVertexShader.h"
#include "OpenGLFragmentShader.h"
#include <Core/Utils/Misc.h>

namespace crystal
{
    static constexpr int MAX_BUFFSIZE = 2048;
    static char infoLog[MAX_BUFFSIZE];

    OpenGLShaderProgram::OpenGLShaderProgram(GraphicsDevice* graphicsDevice, std::shared_ptr<VertexShader> vertexShader, 
        std::shared_ptr<FragmentShader> fragmentShader, const UniformVariableCollection& uniforms)
        : m_pGraphicsDevice(graphicsDevice)
    {
        GLuint id = glCreateProgram();
        glAttachShader(id, vertexShader->GetHandle());
        glAttachShader(id, fragmentShader->GetHandle());
        glLinkProgram(id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, MAX_BUFFSIZE, nullptr, infoLog);
            std::string str = string_format("[%s, %s]", vertexShader->GetName().c_str(),
                fragmentShader->GetName().c_str());
            throw std::exception(string_format("Failed to link shader with %s: %s", str.c_str(), infoLog).c_str());
        }
        m_shaderHandle = id;

        m_constBufferSize = 0;
        for (auto& var : uniforms.Variables)
        {
            GLint loc = glGetUniformLocation(m_shaderHandle, var.Name.c_str());
            m_uniformVariables.push_back({ loc, m_constBufferSize, var.Format });
            m_uniformOffsetTable[var.Name] = m_constBufferSize;
            m_constBufferSize += ComponentFormatToSizeConvert(var.Format);
        }
        m_pConstantBufferData = std::make_unique<char[]>(m_constBufferSize);
	}

    OpenGLShaderProgram::~OpenGLShaderProgram()
	{}

    void OpenGLShaderProgram::Apply()
    {
        glUseProgram(this->GetHandle());
        for (auto& var : m_uniformVariables)
        {
            m_SetOneUniform(var);
        }
    }

    void OpenGLShaderProgram::SetUniform1f(const std::string& name, float value)
    {
        auto iter = m_uniformOffsetTable.find(name);
        if (iter == m_uniformOffsetTable.end())
        {
            throw std::exception("Cannot find uniform variable");
        }
        memcpy_s(&m_pConstantBufferData[iter->second], sizeof(float), &value, sizeof(float));
        m_constBufferDirty = true;
    }

    void OpenGLShaderProgram::SetUniformMat4f(const std::string& name, const glm::mat4& value)
    {
        auto iter = m_uniformOffsetTable.find(name);
        if (iter == m_uniformOffsetTable.end())
        {
            throw std::exception("Cannot find uniform variable");
        }
        memcpy_s(&m_pConstantBufferData[iter->second], sizeof(Matrix4f),
            crystal_value_ptr(value), sizeof(Matrix4f));
        m_constBufferDirty = true;
    }

    void OpenGLShaderProgram::m_SetOneUniform(const GLUniformVariable& var)
    {
        switch (var.Format)
        {
        case ComponentFormat::Single:
        {
            float v = *(float*)&m_pConstantBufferData[var.Offset];
            glUniform1f(var.Location, v);
        }
        return;
        case ComponentFormat::Mat4f:
        {
            float* startPos = (float*)&m_pConstantBufferData[var.Offset];
            glUniformMatrix4fv(var.Location, 1, GL_FALSE, startPos);
        }
        return;
        default:
            break;
        }
        throw std::exception("Unknown variable format");
    }
}