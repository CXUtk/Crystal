#include "OpenGLGraphicsDevice.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexShader.h"
#include "OpenGLFragmentShader.h"
#include "OpenGLShaderProgram.h"
#include "OpenGLPipelineStateObject.h"

#include <Core/Utils/Misc.h>
#include <SJson/SJson.h>

namespace crystal
{
	static constexpr int MAX_BUFFSIZE = 2048;
	static char infoLog[MAX_BUFFSIZE];

	OpenGLGraphicsDevice::OpenGLGraphicsDevice(const InitArgs& args, GLFWGameWindow* window)
		: m_pWindow(window)
	{
		// Initialize GLAD and configs
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::exception("Failed to load glad!");
		}

		window->AppendOnResizeEvent([](Vector2i size) {
			glViewport(0, 0, size.x, size.y);
		});

	}
	
	OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
	{}

	void OpenGLGraphicsDevice::SetPipelineStateObject(std::shared_ptr<PipelineStateObject> pso)
	{
		glBindVertexArray(pso->GetHandle());
		glEnableVertexAttribArray(0);
		glEnable(GL_DEPTH_TEST);
		pso->GetShaderProgram()->Apply();
	}

	void OpenGLGraphicsDevice::Clear(ClearOptions options, const Color4f & color, float depth, int stencil)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		int flags = 0;
		if (options & ClearOptions::Target)
		{
			flags |= GL_COLOR_BUFFER_BIT;
		}
		if (options & ClearOptions::Depth)
		{
			flags |= GL_DEPTH_BUFFER_BIT;
		}
		if (options & ClearOptions::Stencil)
		{
			flags |= GL_STENCIL_BUFFER_BIT;
		}
		glClear(flags);
	}

	void OpenGLGraphicsDevice::Present()
	{
		glfwSwapBuffers(m_pWindow->GetGLFWWindow());
	}

	void OpenGLGraphicsDevice::DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices)
	{
		glDrawArrays(PrimitiveTypeToTopologyConvert(primitiveType), offset, numVertices);
	}

	void OpenGLGraphicsDevice::DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices, size_t indexOffset, size_t vertexOffset)
	{
		
		glDrawElements(PrimitiveTypeToTopologyConvert(primitiveType), numIndices, GL_UNSIGNED_INT, (void*)vertexOffset);
	}

	std::shared_ptr<PipelineStateObject> OpenGLGraphicsDevice::CreatePipelineStateObject()
	{
		GLuint VAO;
		glGenVertexArrays(1, &VAO);

		return std::make_shared<OpenGLPipelineStateObject>(this, VAO);
	}

	std::shared_ptr<VertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(const VertexBufferDescription& desc, void* src, size_t size)
	{
		GLuint VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, src, BufferUsageToOpenGLConvert(desc.Usage));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return std::make_shared<OpenGLVertexBuffer>(this, VBO);
	}

	std::shared_ptr<IndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(const IndexBufferDescription& desc, void* src, size_t size)
	{
		GLuint EBO;
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, src, BufferUsageToOpenGLConvert(desc.Usage));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return std::make_shared<OpenGLIndexBuffer>(this, EBO, desc);
	}

	std::shared_ptr<VertexShader> OpenGLGraphicsDevice::CreateVertexShaderFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint)
	{
		GLuint id = glCreateShader(ShaderTypeToModelConvert(ShaderType::VERTEX_SHADER));
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, MAX_BUFFSIZE, nullptr, infoLog);
			// 需不需要 glDeleteShader(_handle) 呢?
			throw std::exception(string_format("Failed to compile vertex shader %s: %s", name.c_str(), infoLog).c_str());
		}
		return std::make_shared<OpenGLVertexShader>(this, id, name);
	}

	std::shared_ptr<FragmentShader> OpenGLGraphicsDevice::CreateFragmentShaderFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint)
	{
		GLuint id = glCreateShader(ShaderTypeToModelConvert(ShaderType::FRAGMENT_SHADER));
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, MAX_BUFFSIZE, nullptr, infoLog);
			// 需不需要 glDeleteShader(_handle) 呢?
			throw std::exception(string_format("Failed to compile fragment shader %s: %s", name.c_str(), infoLog).c_str());
		}
		return std::make_shared<OpenGLFragmentShader>(this, id, name);
	}

	UniformVariable ParseUniformVariable(const std::string& name, const std::string& type)
	{
		UniformVariable varb;
		varb.Name = name;
		varb.Format = StringToComponentFormatConvert(type);
		return varb;
	}

	std::shared_ptr<ShaderProgram> OpenGLGraphicsDevice::CreateShaderProgramFromFile(const std::string& path)
	{
		auto source = ReadAllStringFromFile(path);
		auto root = SJson::SJsonParse(source);
		auto srcVS = root->GetMember("openGLVSSrc");
		auto srcFS = root->GetMember("openGLFSSrc");
		auto directory = GetDirectoryPath(path);

		auto filePathVS = directory + "/" + srcVS->GetString();
		auto shaderSrcVS = ReadAllStringFromFile(filePathVS);

		auto filePathFS = directory + "/" + srcFS->GetString();
		auto shaderSrcFS = ReadAllStringFromFile(filePathFS);

		UniformVariableCollection variables;
		auto uniforms = root->GetMember("uniforms");
		uniforms->ForEachElements([&variables](const SJson::SJsonNode* node)
		{
			UniformVariable variable = ParseUniformVariable(node->GetMember("name")->GetString(),
				node->GetMember("type")->GetString());
			variables.Add(variable);
		});
		auto vs = this->CreateVertexShaderFromMemory(shaderSrcVS.c_str(), shaderSrcVS.size(),
			srcVS->GetString(), "main");
		auto fs = this->CreateFragmentShaderFromMemory(shaderSrcFS.c_str(), shaderSrcFS.size(),
			srcFS->GetString(), "main");

		return std::make_shared<OpenGLShaderProgram>(this, vs, fs, variables);
	}
}
