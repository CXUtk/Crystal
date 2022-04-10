#pragma once
#include <memory>
#include <Interfaces/Interfaces.h>
#include <SJson/SJson.hpp>
#include <Interfaces/FileSystem/File.h>

namespace crystal
{
	class ShaderLoader
	{
	public:
		
		static std::shared_ptr<IShaderProgram> LoadShader(const SJson::JsonNode& metaData,
			const path_type& curPath);

	private:
		static std::shared_ptr<IVertexShader> LoadVertexShader(const SJson::JsonNode& metaData, const path_type& curPath);
		static std::shared_ptr<IFragmentShader> LoadFragmentShader(const SJson::JsonNode& metaData, const path_type& curPath);
	};
}