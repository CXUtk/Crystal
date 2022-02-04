#include "DX11Common.h"
#include <array>
#include <map>
#include <Core/Platform/Graphics/GraphicsCommon.h>
#include <Core/Utils/Geometry.h>

namespace crystal
{

	Matrix4f crystal_look_at(const Point3f& eye, const Point3f& center, const Vector3f& up)
	{
		return glm::lookAtRH(eye, center, up);
	}
	Matrix4f crystal_perspective(Float fovY, Float aspectRatio, Float zNear, Float zFar)
	{
		return glm::perspectiveRH_ZO(fovY, aspectRatio, zNear, zFar);
	}

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
	constexpr std::array<D3D11_USAGE, N> GenerateBufferUsageToDX11Mapping()
	{
		std::array<D3D11_USAGE, N> M{};
		M[(int)BufferUsage::Default] = D3D11_USAGE_DEFAULT;
		M[(int)BufferUsage::Immutable] = D3D11_USAGE_IMMUTABLE;
		M[(int)BufferUsage::CPURead] = D3D11_USAGE_DYNAMIC;
		M[(int)BufferUsage::CPUWrite] = D3D11_USAGE_DYNAMIC;
		M[(int)BufferUsage::CPURWrite] = D3D11_USAGE_DYNAMIC;
		return M;
	}
	constexpr auto BufferUsageToDX11Mapping = GenerateBufferUsageToDX11Mapping<(size_t)BufferUsage::__COUNT>();

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
	constexpr std::array<DXGI_FORMAT, N> GenerateDataFormatToShaderVarMapping()
	{
		std::array<DXGI_FORMAT, N> M{};
		M[(int)DataFormat::Byte8] = DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
		M[(int)DataFormat::SByte8] = DXGI_FORMAT::DXGI_FORMAT_R8_SINT;
		M[(int)DataFormat::UShort16] = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		M[(int)DataFormat::Short16] = DXGI_FORMAT::DXGI_FORMAT_R16_SINT;
		M[(int)DataFormat::UInt32] = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		M[(int)DataFormat::Int32] = DXGI_FORMAT::DXGI_FORMAT_R32_SINT;		
		M[(int)DataFormat::Float32] = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		return M;
	}
	constexpr auto DataFormatMapping = GenerateDataFormatToShaderVarMapping<(size_t)DataFormat::__COUNT>();

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

	template<size_t N>
	constexpr std::array<D3D11_PRIMITIVE_TOPOLOGY, N> GeneratePrimititveTopologyMapping()
	{
		std::array<D3D11_PRIMITIVE_TOPOLOGY, N> M{};
		M[(int)PrimitiveType::POINTS] = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		M[(int)PrimitiveType::LINE_LIST] = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		M[(int)PrimitiveType::LINE_STRIP] = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		M[(int)PrimitiveType::TRIANGLE_LIST] = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		M[(int)PrimitiveType::TRIANGLE_STRIP] = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		return M;
	}
	constexpr auto PrimititveTopologyMapping = GeneratePrimititveTopologyMapping<(size_t)PrimitiveType::__COUNT>();


	template<size_t N>
	constexpr std::array<D3D11_CULL_MODE, N> GenerateCullModeMapping()
	{
		std::array<D3D11_CULL_MODE, N> M{};
		M[(int)CullingMode::None] = D3D11_CULL_MODE::D3D11_CULL_NONE;
		M[(int)CullingMode::CullCCW] = D3D11_CULL_MODE::D3D11_CULL_BACK;
		M[(int)CullingMode::CullCW] = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		return M;
	}
	constexpr auto CullModeMapping = GenerateCullModeMapping<(size_t)CullingMode::__COUNT>();

	template<size_t N>
	constexpr std::array<D3D11_FILL_MODE, N> GenerateFillModeMapping()
	{
		std::array<D3D11_FILL_MODE, N> M{};
		M[(int)FillMode::SOLID] = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		M[(int)FillMode::WIREFRAME] = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		return M;
	}
	constexpr auto FillModeMapping = GenerateFillModeMapping<(size_t)FillMode::__COUNT>();

	void InitDX11Commons()
	{
		static bool initialized = false;
		if (initialized) return;
		{
		}
		initialized = true;
	}

	const char* VertexElementFormatToShaderVarConvert(VertexElementFormat format)
	{
		return VertexElementFormatToShaderVarMapping[(int)format];
	}

	DXGI_FORMAT VertexElementFormatConvert(VertexElementFormat format)
	{
		return VertexElementFormatMapping[(int)format];
	}

	DXGI_FORMAT DataFormatConvert(DataFormat format)
	{
		return DataFormatMapping[(int)format];
	}

	D3D11_USAGE BufferUsageToDX11Convert(BufferUsage usage)
	{
		return BufferUsageToDX11Mapping[(int)usage];
	}

	const char* SemanticNameConvert(SemanticType semanticType)
	{
		return SemanticNameMapping[(int)semanticType];
	}

	const char* ShaderModelConvert(ShaderType shaderType)
	{
		return ShaderTypeToModelStringMapping[(int)shaderType];
	}

	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTypeToTopologyConvert(PrimitiveType type)
	{
		return PrimititveTopologyMapping[(int)type];
	}

	D3D11_CULL_MODE CullModeConvert(CullingMode mode)
	{
		return CullModeMapping[(int)mode];
	}

	D3D11_FILL_MODE FillModeConvert(FillMode mode)
	{
		return FillModeMapping[(int)mode];
	}
}