#include "DX11Common.h"
#include <array>

namespace crystal
{

	template<size_t N>
	constexpr std::array<DXGI_FORMAT, N> GenerateVertexElementFormatMapping()
	{
		std::array<DXGI_FORMAT, N> M{};
		M[(int)VertexElementFormat::Single] = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		M[(int)VertexElementFormat::Vector2] = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		M[(int)VertexElementFormat::Vector3] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		M[(int)VertexElementFormat::Vector4] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		M[(int)VertexElementFormat::R8_UINT] = DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
		M[(int)VertexElementFormat::R8G8B8A8_UINT] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UINT;
		M[(int)VertexElementFormat::R32G32B32_UINT] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
		return M;
	}
	constexpr auto VertexElementFormatMapping = GenerateVertexElementFormatMapping<(size_t)VertexElementFormat::__COUNT>();

	template<size_t N>
	constexpr std::array<const char*, N> GenerateVertexElementFormatToShaderVarMapping()
	{
		std::array<const char*, N> M{};
		M[(int)VertexElementFormat::Single] = "float";
		M[(int)VertexElementFormat::Vector2] = "float2";
		M[(int)VertexElementFormat::Vector3] = "float3";
		M[(int)VertexElementFormat::Vector4] = "float4";
		M[(int)VertexElementFormat::R8_UINT] = "float2";
		M[(int)VertexElementFormat::R8G8B8A8_UINT] = "uint4";
		M[(int)VertexElementFormat::R32G32B32_UINT] = "uint3";
		return M;
	}
	constexpr auto VertexElementFormatToShaderVarMapping = GenerateVertexElementFormatToShaderVarMapping<(size_t)VertexElementFormat::__COUNT>();

	template<size_t N>
	constexpr std::array<const char*, N> GenerateShaderTypeToModelStringMapping()
	{
		std::array<const char*, N> M{};
		M[(int)ShaderType::VERTEX_SHADER] = "vs_5_0";
		M[(int)ShaderType::FRAGMENT_SHADER] = "ps_5_0";
		M[(int)ShaderType::GEOMETRY_SHADER] = "gs_5_0";
		M[(int)ShaderType::COMPUTE_SHADER] = "cs_5_0";
		return M;
	}
	constexpr auto ShaderTypeToModelStringMapping = GenerateShaderTypeToModelStringMapping<(size_t)ShaderType::__COUNT>();

	template<size_t N>
	constexpr std::array<const char*, N> GenerateSemanticNameMapping()
	{
		std::array<const char*, N> M{};
		M[(int)SemanticType::POSITION] = "POSITION";
		M[(int)SemanticType::COLOR] = "COLOR";
		M[(int)SemanticType::TEXCOORD] = "TEXCOORD";
		M[(int)SemanticType::NORMAL] = "NORMAL";
		return M;
	}
	constexpr auto SemanticNameMapping = GenerateSemanticNameMapping<(size_t)SemanticType::__COUNT>();

	const char* VertexElementFormatToShaderVarConvert(VertexElementFormat format)
	{
		return VertexElementFormatToShaderVarMapping[(int)format];
	}

	DXGI_FORMAT VertexElementFormatConvert(VertexElementFormat format)
	{
		return VertexElementFormatMapping[(int)format];
	}

	const char* SemanticNameConvert(SemanticType semanticType)
	{
		return SemanticNameMapping[(int)semanticType];
	}

	const char* ShaderModelConvert(ShaderType shaderType)
	{
		return ShaderTypeToModelStringMapping[(int)shaderType];
	}

}