#include "GraphicsCommon.h"
#include <array>

namespace crystal
{
	template<size_t N>
	constexpr std::array<size_t, N> GenerateComponentFormatToSizeMapping()
	{
		std::array<size_t, N> M{};
		M[(int)ComponentFormat::Int] = sizeof(int);
		M[(int)ComponentFormat::Single] = sizeof(float);
		M[(int)ComponentFormat::Vector2f] = 2 * sizeof(float);
		M[(int)ComponentFormat::Vector2i] = 2 * sizeof(int);
		M[(int)ComponentFormat::Vector3f] = 3 * sizeof(float);
		M[(int)ComponentFormat::Vector3i] = 3 * sizeof(int);
		M[(int)ComponentFormat::Vector4f] = 4 * sizeof(float);
		M[(int)ComponentFormat::Vector4i] = 4 * sizeof(int);
		M[(int)ComponentFormat::Mat2f] = 4 * sizeof(float);
		M[(int)ComponentFormat::Mat3f] = 9 * sizeof(float);
		M[(int)ComponentFormat::Mat4f] = 16 * sizeof(float);
		return M;
	}
	static constexpr auto ComponentFormatToSizeMapping = GenerateComponentFormatToSizeMapping<(size_t)ComponentFormat::__COUNT>();


	static std::map<std::string, ComponentFormat> StringToComponentFormatMapping;
	static std::map<std::string, DataFormat> StringToDataFormatMapping;

	static void GenerateStringToComponentFormatMapping()
	{
		StringToComponentFormatMapping["float"] = ComponentFormat::Single;
		StringToComponentFormatMapping["vec2"] = ComponentFormat::Vector2f;
		StringToComponentFormatMapping["vec3"] = ComponentFormat::Vector3f;
		StringToComponentFormatMapping["vec4"] = ComponentFormat::Vector4f;
		StringToComponentFormatMapping["int"] = ComponentFormat::Int;
		StringToComponentFormatMapping["ivec2"] = ComponentFormat::Vector2i;
		StringToComponentFormatMapping["ivec3"] = ComponentFormat::Vector3i;
		StringToComponentFormatMapping["ivec4"] = ComponentFormat::Vector4i;
		StringToComponentFormatMapping["mat2"] = ComponentFormat::Mat2f;
		StringToComponentFormatMapping["mat3"] = ComponentFormat::Mat3f;
		StringToComponentFormatMapping["mat4"] = ComponentFormat::Mat4f;
	}

	static void GenerateStringToDataFormatMapping()
	{
		StringToDataFormatMapping["float"] = DataFormat::Float32;
		StringToDataFormatMapping["vec2"] = DataFormat::Float32;
		StringToDataFormatMapping["vec3"] = DataFormat::Float32;
		StringToDataFormatMapping["vec4"] = DataFormat::Float32;
		StringToDataFormatMapping["int"] = DataFormat::Int32;
		StringToDataFormatMapping["ivec2"] = DataFormat::Int32;
		StringToDataFormatMapping["ivec3"] = DataFormat::Int32;
		StringToDataFormatMapping["ivec4"] = DataFormat::Int32;
		StringToDataFormatMapping["mat2"] = DataFormat::Float32;
		StringToDataFormatMapping["mat3"] = DataFormat::Float32;
		StringToDataFormatMapping["mat4"] = DataFormat::Float32;
	}

	DataFormat GraphicsCommons::StringToDataFormatConvert(const std::string& type)
	{
		auto iter = StringToDataFormatMapping.find(type);
		if (iter == StringToDataFormatMapping.end())
		{
			throw std::exception("Unknown data type");
		}
		return iter->second;
	}

	size_t GraphicsCommons::ComponentFormatToSizeConvert(ComponentFormat format)
	{
		return ComponentFormatToSizeMapping[(int)format];
	}

	ComponentFormat GraphicsCommons::StringToComponentFormatConvert(const std::string& type)
	{
		auto iter = StringToComponentFormatMapping.find(type);
		if (iter == StringToComponentFormatMapping.end())
		{
			throw std::exception("Unknown data type");
		}
		return iter->second;
	}

	void GraphicsCommons::InitGraphicsCommons()
	{
		static bool initialized = false;
		if (initialized) return;
		{
			GenerateStringToComponentFormatMapping(); 
			GenerateStringToDataFormatMapping();
		}
		initialized = true;
	}
}