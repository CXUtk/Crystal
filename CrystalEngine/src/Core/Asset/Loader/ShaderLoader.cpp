#include "ShaderLoader.h"
#include <Engine.h>


namespace crystal
{

	UniformVariable ParseUniformVariable(const std::string& name, const std::string& type)
	{
		UniformVariable varb;
		varb.Name = name;
		varb.Format = GraphicsCommons::StringToComponentFormatConvert(type);
		return varb;
	}

	std::shared_ptr<IShaderProgram> ShaderLoader::LoadShader(const SJson::JsonNode& metaData, const path_type& curPath)
	{
		auto engine = Engine::GetInstance();
		auto graphicsDevice = engine->GetGraphicsDevice();
		UniformVariableCollection variables;
		auto& uniforms = metaData["uniforms"];
		uniforms.foreach([&variables](const SJson::JsonNode& node)
		{
			UniformVariable variable = ParseUniformVariable(node["name"].Get<std::string>(),
				node["type"].Get<std::string>());
			variables.Add(variable);
		});

		auto vs = LoadVertexShader(metaData, curPath);
		auto fs = LoadFragmentShader(metaData, curPath);
		return graphicsDevice->CreateShaderProgram(vs, fs, variables);
	}

	std::shared_ptr<IVertexShader> ShaderLoader::LoadVertexShader(const SJson::JsonNode& metaData, const path_type& curPath)
	{
		auto engine = Engine::GetInstance();
		auto graphicsDevice = engine->GetGraphicsDevice();
		auto graphicsAPI = engine->GetGraphicsAPIType();

		path_type pathToVertexShader = curPath;
		std::string fileName;

		if (graphicsAPI == GraphicsAPIType::DirectX11 || graphicsAPI == GraphicsAPIType::DirectX12)
		{
			fileName = metaData["dx11Src"].Get<std::string>();
		}
		else if (graphicsAPI == GraphicsAPIType::OpenGL)
		{
			fileName = metaData["openGLVSSrc"].Get<std::string>();
		}

		pathToVertexShader /= fileName;

		auto source = File::ReadAllText(pathToVertexShader);

		if (graphicsAPI == GraphicsAPIType::DirectX11 || graphicsAPI == GraphicsAPIType::DirectX12)
		{
			return graphicsDevice->CreateVertexShaderFromMemory(source.c_str(), source.size(),
				fileName, metaData["vsEntry"].Get<std::string>());
		}
		else if (graphicsAPI == GraphicsAPIType::OpenGL)
		{
			return graphicsDevice->CreateVertexShaderFromMemory(source.c_str(), source.size(),
							fileName, "");
		}
		else
		{
			throw std::exception("Unknown graphics API");
		}
	}


	std::shared_ptr<IFragmentShader> ShaderLoader::LoadFragmentShader(const SJson::JsonNode& metaData, const path_type& curPath)
	{
		auto engine = Engine::GetInstance();
		auto graphicsDevice = engine->GetGraphicsDevice();
		auto graphicsAPI = engine->GetGraphicsAPIType();

		path_type pathToFragmentShader = curPath;
		std::string fileName;

		if (graphicsAPI == GraphicsAPIType::DirectX11 || graphicsAPI == GraphicsAPIType::DirectX12)
		{
			fileName = metaData["dx11Src"].Get<std::string>();
		}
		else if (graphicsAPI == GraphicsAPIType::OpenGL)
		{
			fileName = metaData["openGLVSSrc"].Get<std::string>();
		}

        pathToFragmentShader /= fileName;

		auto source = File::ReadAllText(pathToFragmentShader);

		if (graphicsAPI == GraphicsAPIType::DirectX11 || graphicsAPI == GraphicsAPIType::DirectX12)
		{
			return graphicsDevice->CreateFragmentShaderFromMemory(source.c_str(), source.size(),
				fileName, metaData["fsEntry"].Get<std::string>());
		}
		else if (graphicsAPI == GraphicsAPIType::OpenGL)
		{
			return graphicsDevice->CreateFragmentShaderFromMemory(source.c_str(), source.size(),
							fileName, "");
		}
		else
		{
			throw std::exception("Unknown graphics API");
		}
	}
}
