#include "DX11Common.h"
#include <array>
#include <map>
#include <Interfaces/Graphics/GraphicsCommon.h>
#include <Core/Utils/Geometry.h>

namespace crystal
{

	Matrix4f crystal_look_at(const Point3f& eye, const Point3f& center, const Vector3f& up)
	{
		return glm::lookAtLH(eye, center, up);
	}
	Matrix4f crystal_perspective(Float fovY, Float aspectRatio, Float zNear, Float zFar)
	{
		return glm::perspectiveLH_ZO(fovY, aspectRatio, zNear, zFar);
	}

	template<size_t N>
	constexpr std::array<DXGI_FORMAT, N> GenerateRenderFormatMapping()
	{
		std::array<DXGI_FORMAT, N> M{};
		M[(int)RenderFormat::R32f] = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		M[(int)RenderFormat::RG32f] = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		M[(int)RenderFormat::RGB32f] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		M[(int)RenderFormat::RGBA32f] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		M[(int)RenderFormat::R8ub] = DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
		M[(int)RenderFormat::RG8ub] = DXGI_FORMAT::DXGI_FORMAT_R8G8_UINT;
		M[(int)RenderFormat::RGBA8ub] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UINT;
		M[(int)RenderFormat::R32ui] = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		M[(int)RenderFormat::RG32ui] = DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
		M[(int)RenderFormat::RGB32ui] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
		M[(int)RenderFormat::RGBA32ui] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
		return M;
	}
	constexpr auto RenderFormatMapping = GenerateRenderFormatMapping<(size_t)RenderFormat::__COUNT>();

	template<size_t N>
	constexpr std::array<DXGI_FORMAT, N> GenerateRenderFormatMappingNormal()
	{
		std::array<DXGI_FORMAT, N> M{};
		M[(int)RenderFormat::R32f] = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		M[(int)RenderFormat::RG32f] = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		M[(int)RenderFormat::RGB32f] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		M[(int)RenderFormat::RGBA32f] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		M[(int)RenderFormat::R8ub] = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM;
		M[(int)RenderFormat::RG8ub] = DXGI_FORMAT::DXGI_FORMAT_R8G8_UNORM;
		M[(int)RenderFormat::RGBA8ub] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		M[(int)RenderFormat::R32ui] = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		M[(int)RenderFormat::RG32ui] = DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
		M[(int)RenderFormat::RGB32ui] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
		M[(int)RenderFormat::RGBA32ui] = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
		return M;
	}
	constexpr auto RenderFormatMapping_Normal = GenerateRenderFormatMappingNormal<(size_t)RenderFormat::__COUNT>();

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
	constexpr std::array<size_t, N> GenerateRenderFormatToSizeInBytesMapping()
	{
		std::array<size_t, N> M{};
		M[(int)RenderFormat::R32f] = sizeof(float) * 1;
		M[(int)RenderFormat::RG32f] = sizeof(float) * 2;
		M[(int)RenderFormat::RGB32f] = sizeof(float) * 3;
		M[(int)RenderFormat::RGBA32f] = sizeof(float) * 4;
		M[(int)RenderFormat::R8ub] = sizeof(char) * 1;
		M[(int)RenderFormat::RG8ub] = sizeof(char) * 2;
		M[(int)RenderFormat::RGBA8ub] = sizeof(char) * 4;
		M[(int)RenderFormat::R32ui] = sizeof(int) * 1;
		M[(int)RenderFormat::RG32ui] = sizeof(int) * 2;
		M[(int)RenderFormat::RGB32ui] = sizeof(int) * 3;
		M[(int)RenderFormat::RGBA32ui] = sizeof(int) * 4;
		return M;
	}
	constexpr auto RenderFormatToSizeInBytesMapping = GenerateRenderFormatToSizeInBytesMapping<(size_t)RenderFormat::__COUNT>();

	template<size_t N>
	constexpr std::array<const char*, N> GenerateRenderFormatToShaderVarMapping()
	{
		std::array<const char*, N> M{};
		M[(int)RenderFormat::R32f] = "float";
		M[(int)RenderFormat::RG32f] = "float2";
		M[(int)RenderFormat::RGB32f] = "float3";
		M[(int)RenderFormat::RGBA32f] = "float4";
		M[(int)RenderFormat::R8ub] = "byte";
		M[(int)RenderFormat::RG8ub] = "byte2";
		M[(int)RenderFormat::RGBA8ub] = "byte4";
		M[(int)RenderFormat::R32ui] = "uint";
		M[(int)RenderFormat::RG32ui] = "uint2";
		M[(int)RenderFormat::RGB32ui] = "uint3";
		M[(int)RenderFormat::RGBA32ui] = "uint4";
		return M;
	}
	constexpr auto RenderFormatToShaderVarMapping = GenerateRenderFormatToShaderVarMapping<(size_t)RenderFormat::__COUNT>();

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

	template<size_t N>
	constexpr std::array<D3D11_BLEND, N> GenerateBlendFactorMapping()
	{
		std::array<D3D11_BLEND, N> M{};
		M[(int)BlendFactors::Zero] = D3D11_BLEND::D3D11_BLEND_ZERO;
		M[(int)BlendFactors::One] = D3D11_BLEND::D3D11_BLEND_ONE;
		M[(int)BlendFactors::SrcColor] = D3D11_BLEND::D3D11_BLEND_SRC_COLOR;		
		M[(int)BlendFactors::InvSrcColor] = D3D11_BLEND::D3D11_BLEND_INV_SRC_COLOR;
		M[(int)BlendFactors::SrcAlpha] = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		M[(int)BlendFactors::InvSrcAlpha] = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		M[(int)BlendFactors::DestColor] = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
		M[(int)BlendFactors::InvDestColor] = D3D11_BLEND::D3D11_BLEND_INV_DEST_COLOR;
		M[(int)BlendFactors::DestAlpha] = D3D11_BLEND::D3D11_BLEND_DEST_ALPHA;
		M[(int)BlendFactors::InvDestAlpha] = D3D11_BLEND::D3D11_BLEND_INV_DEST_ALPHA;
		return M;
	}
	constexpr auto BlendFactorMapping = GenerateBlendFactorMapping<(size_t)BlendFactors::__COUNT>();

	template<size_t N>
	constexpr std::array<D3D11_BLEND_OP, N> GenerateBlendOpMapping()
	{
		std::array<D3D11_BLEND_OP, N> M{};
		M[(int)BlendOperations::Add] = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		M[(int)BlendOperations::Subtract] = D3D11_BLEND_OP::D3D11_BLEND_OP_SUBTRACT;
		M[(int)BlendOperations::RevSubtract] = D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT;
		M[(int)BlendOperations::Min] = D3D11_BLEND_OP::D3D11_BLEND_OP_MIN;
		M[(int)BlendOperations::Max] = D3D11_BLEND_OP::D3D11_BLEND_OP_MAX;
		return M;
	}
	constexpr auto BlendOpMapping = GenerateBlendOpMapping<(size_t)BlendOperations::__COUNT>();

	void DX11Common::InitDX11Commons(DX11GraphicsDevice* graphicsDevice)
	{
		static bool initialized = false;
		if (initialized) return;
		{
		}
		initialized = true;
	}

	const char* DX11Common::RenderFormatToShaderVarConvert(RenderFormat format)
	{
		return RenderFormatToShaderVarMapping[(int)format];
	}

	DXGI_FORMAT DX11Common::RenderFormatConvert(RenderFormat format, bool normalized)
	{
		return  normalized ? RenderFormatMapping_Normal[(int)format] : RenderFormatMapping[(int)format];
	}

	size_t DX11Common::RenderFormatToBytes(RenderFormat format)
	{
		return RenderFormatToSizeInBytesMapping[(int)format];
	}

	DXGI_FORMAT DX11Common::DataFormatConvert(DataFormat format)
	{
		return DataFormatMapping[(int)format];
	}

	D3D11_USAGE DX11Common::BufferUsageToDX11Convert(BufferUsage usage)
	{
		return BufferUsageToDX11Mapping[(int)usage];
	}

	const char* DX11Common::SemanticNameConvert(SemanticType semanticType)
	{
		return SemanticNameMapping[(int)semanticType];
	}

	const char* DX11Common::ShaderModelConvert(ShaderType shaderType)
	{
		return ShaderTypeToModelStringMapping[(int)shaderType];
	}

	D3D11_PRIMITIVE_TOPOLOGY DX11Common::PrimitiveTypeToTopologyConvert(PrimitiveType type)
	{
		return PrimititveTopologyMapping[(int)type];
	}

	D3D11_CULL_MODE DX11Common::CullModeConvert(CullingMode mode)
	{
		return CullModeMapping[(int)mode];
	}

	D3D11_FILL_MODE DX11Common::FillModeConvert(FillMode mode)
	{
		return FillModeMapping[(int)mode];
	}
	D3D11_BLEND DX11Common::BlendFactorConvert(BlendFactors blendFactor)
	{
		return BlendFactorMapping[(int)blendFactor];
	}
	D3D11_BLEND_OP DX11Common::BlendOpConvert(BlendOperations blendOp)
	{
		return BlendOpMapping[(int)blendOp];
	}
	D3D11_COMPARISON_FUNC DX11Common::ComparisonFunctionConvert(ComparisonFunction cmp)
	{
		switch (cmp)
		{
		case crystal::ComparisonFunction::Never:
			return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		case crystal::ComparisonFunction::Always:
			return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		case crystal::ComparisonFunction::Equal:
			return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL;
		case crystal::ComparisonFunction::NotEqual:
			return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL;
		case crystal::ComparisonFunction::Less:
			return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		case crystal::ComparisonFunction::LessEqual:
			return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		case crystal::ComparisonFunction::Greater:
			return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		case crystal::ComparisonFunction::GreaterEqual:
			return D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER_EQUAL;
		default:
			break;
		}
		throw std::logic_error("Invalid Comparison Function type");
	}
	D3D11_STENCIL_OP DX11Common::StencilOperationConvert(StencilOperation op)
	{
		switch (op)
		{
		case crystal::StencilOperation::Zero:
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO;
		case crystal::StencilOperation::Keep:
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		case crystal::StencilOperation::Replace:
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE;
		case crystal::StencilOperation::IncreaseClamp:
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;
		case crystal::StencilOperation::DecreaseClamp:
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT;
		case crystal::StencilOperation::IncreaseWarp:
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
		case crystal::StencilOperation::DecreaseWarp:
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR;
		case crystal::StencilOperation::Invert:
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT;
		default:
			break;
		}
		throw std::logic_error("Invalid Stencil Operation type");
	}
}