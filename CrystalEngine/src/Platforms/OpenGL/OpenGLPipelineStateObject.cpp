#include "OpenGLPipelineStateObject.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLShaderProgram.h"

namespace crystal
{
	OpenGLPipelineStateObject::OpenGLPipelineStateObject(GraphicsDevice* graphicsDevice, GLuint handle)
		: m_pGraphicsDevice(graphicsDevice), m_VAOHandle(handle)
	{

	}

	OpenGLPipelineStateObject::~OpenGLPipelineStateObject()
	{}

	void SetAttribPointer(int& index, const ElementDescription& desc, size_t stride)
	{
		switch (desc.Format)
		{
		case VertexElementFormat::Single:
		{
			glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, stride, (void*)desc.ByteOffset);
			glEnableVertexAttribArray(index);
			index++;
		}
		return;
		case VertexElementFormat::Vector2:
		{
			glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, (void*)desc.ByteOffset);
			glEnableVertexAttribArray(index);
			index++;
		}
		return;
		case VertexElementFormat::Vector3:
		{
			glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (void*)desc.ByteOffset);
			glEnableVertexAttribArray(index);
			index++;
		}
		return;
		case VertexElementFormat::Vector4:
		{
			glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, (void*)desc.ByteOffset);
			glEnableVertexAttribArray(index);
			index++;
		}
		return;
		default:
			break;
		}
		throw std::exception("Unknown vertex element format");
	}

	void OpenGLPipelineStateObject::BindVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		m_vertexBuffer = vertexBuffer;
		glBindVertexArray(m_VAOHandle);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetHandle());
		
		auto &vertexLayout = vertexBuffer->GetVertexLayout();
		if (vertexLayout.Elements.size() == 0)
		{
			throw std::exception("No vertexLayout was bounded with VertexBuffer");
		}
		int index = 0;
		for (auto& desc : vertexLayout.Elements)
		{
			SetAttribPointer(index, desc, vertexLayout.Stride);
		}
		glBindVertexArray(0);
	}

	void OpenGLPipelineStateObject::BindIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
	{
		m_indexBuffer = indexBuffer;
		glBindVertexArray(m_VAOHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetHandle());
		glBindVertexArray(0);
	}

	void OpenGLPipelineStateObject::BindShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram)
	{
		m_shaderProgram = shaderProgram;
	}
}