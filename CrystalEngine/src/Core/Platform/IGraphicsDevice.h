#pragma once
#include <Crystal.h>
#include <vector>

namespace crystal
{
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

	enum class BufferHint
	{
		STATIC_DRAW,
		DYNAMIC_DRAW,

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


	struct VertexElement
	{
		int Offset;
		int NumberOfElements;
		VertexDataType DataType;
		bool Normalized;

		VertexElement(int offset, int size, EngineDataType dataType, bool normalized)
			: Offset(offset), NumberOfElements(size), DataType(dataType), Normalized(normalized)
		{

		}
	};

	class VertexLayout
	{
	public:
		VertexLayout();
		~VertexLayout();

		void Add(const VertexElement& element);
		size_t GetSize() const;
		const std::vector<VertexElement>& GetVertexElements() const { return _vertexElements; }

	private:
		size_t _size = 0;
		std::vector<VertexElement> _vertexElements{};
	};


	/**
	 * @brief Graphics device interface. Includes platform independent operations to create and draw rendering objects.
	*/
	class IGraphicsDevice
	{
	public:
		virtual ~IGraphicsDevice() = 0 {};

	};

}