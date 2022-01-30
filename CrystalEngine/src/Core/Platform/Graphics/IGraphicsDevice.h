#pragma once
#include <Crystal.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

namespace crystal
{
	using Spectrum = glm::vec3;
	using Color3f = glm::vec3;
	using Color4f = glm::vec4;

	template<typename T>
	inline float* value_ptr(T x) { return glm::value_ptr(T); }

	enum class VertexDataType
	{
		FLOAT,
		INT,
		UNSIGNED_INT,
		SHORT,
		UNSIGNED_SHORT,
		BYTE,
		UNSIGNED_BYTE,

		__COUNT
	};

	enum class BufferUsage
	{
		GPU_DYNAMIC_DRAW,
		IMMUTABLE_DRAW,
		CPU_DYNAMIC_DRAW,

		__COUNT
	};

	enum class BufferType
	{
		ARRAY_BUFFER,
		INDEX_BUFFER,

		__COUNT
	};

	enum class PrimitiveType
	{
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		LINE_LIST,
		LINE_STRIP,
		POINTS,

		__COUNT
	};

	enum class ShaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		GEOMETRY_SHADER,
		TESSELATION_SHADER,
		COMPUTE_SHADER,

		__COUNT
	};

	enum class VertexElementSemantic
	{
		POSITION = 0,
		NORMAL,
		BINORMAL,
		TANGENT,
		TEXCOORD,
		COLOR,

		__COUNT
	};


	enum class PixelFormat
	{
		RED,
		RG,
		RGB,
		RGBA,
		RGB16F,
		DEPTH,
		STENCIL,
		DEPTH_STENCIL,

		__COUNT
	};

	enum class TextureSampleMethod
	{
		NEAREST,
		BI_LINEAR,
		BI_LINEAR_MIPMAP,

		__COUNT
	};

	enum class TextureWarpMethod
	{
		REPEAT,
		CLAMP_TO_EDGE,
		MIRRORED_REPEAT,

		__COUNT
	};

	enum class BlendingMode
	{
		None,
		AlphaBlend,
		Additive,

		__COUNT
	};

	enum class DepthTestingMode
	{
		None,
		DepthTest,
		DepthTestNoApply,

		__COUNT
	};

	enum class CullingMode
	{
		None,
		CullCCW,
		CullCW,

		__COUNT
	};

	enum class PolygonMode
	{
		WIREFRAME,
		FILL,

		__COUNT
	};


	enum class DepthTestingFunction
	{
		ALWAYS,
		NEVER,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NEQUAL,
		GEQUAL,

		__COUNT
	};

	enum class VertexElementFormat
	{
		Single,
		Vector2,
		Vector3,
		Vector4,

		R8_UINT,
		R32G32B32_UINT,
		R8G8B8A8_UINT,

		__COUNT
	};


	enum class SemanticType
	{
		POSITION,
		NORMAL,
		TEXCOORD,
		COLOR,

		__COUNT
	};

	enum ClearOptions : int
	{
		Target = 1 << 0,
		Depth = 1 << 1,
		Stencil = 1 << 2
	};

	inline ClearOptions operator|(ClearOptions a, ClearOptions b)
	{
		return static_cast<ClearOptions>(static_cast<int>(a) | static_cast<int>(b));
	}


	struct BufferDescription
	{
		BufferType		Type;
		BufferUsage		Usage;
	};

	struct ElementDescription
	{
		SemanticType		Semantic;
		uint32_t			SemanticIndex;
		VertexElementFormat	Format;
		uint32_t			ByteOffset;
	};


	//struct VertexElement
	//{
	//	int Offset;
	//	int NumberOfElements;
	//	VertexDataType DataType;
	//	bool Normalized;

	//	VertexElement(int offset, int size, EngineDataType dataType, bool normalized)
	//		: Offset(offset), NumberOfElements(size), DataType(dataType), Normalized(normalized)
	//	{

	//	}
	//};

	struct VertexLayout
	{
		VertexLayout(const std::vector<ElementDescription>& elementDescs)
			: Elements(elementDescs)
		{}

		std::vector<ElementDescription> Elements{};
	};


	/**
	 * @brief Graphics device interface. Includes platform independent operations to create and draw rendering objects.
	*/
	class IGraphicsDevice
	{
	public:
		virtual ~IGraphicsDevice() = 0 {};

		virtual void Clear(ClearOptions options, const Color4f& color, float depth, int stencil) = 0;
		virtual void Present() = 0;
		virtual std::shared_ptr<IVertexBuffer> CreateBuffer(const BufferDescription& desc, void* src, uint64_t size) = 0;
	};



	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = 0 {};

		virtual void BindVertexLayout(const VertexLayout& layout) = 0;
	};

}