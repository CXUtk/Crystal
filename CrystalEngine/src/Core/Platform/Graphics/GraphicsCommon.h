#pragma once
#include <Crystal.h>
#include <vector>
#include <string>
#include <map>
#include <glm/gtc/type_ptr.hpp>
#include <Core/Utils/Geometry.h>

namespace crystal
{
#ifdef CRYSTAL_USE_OPENGL
	class OpenGLGraphicsDevice;
	using GraphicsDevice = OpenGLGraphicsDevice;

	class OpenGLVertexBuffer;
	using VertexBuffer = OpenGLVertexBuffer;

	class OpenGLIndexBuffer;
	using IndexBuffer = OpenGLIndexBuffer;

	class OpenGLVertexShader;
	using VertexShader = OpenGLVertexShader;

	class OpenGLFragmentShader;
	using FragmentShader = OpenGLFragmentShader;

	class OpenGLShaderProgram;
	using ShaderProgram = OpenGLShaderProgram;

	class OpenGLPipelineStateObject;
	using PipelineStateObject = OpenGLPipelineStateObject;
#elif defined(CRYSTAL_USE_DX11)
	class DX11GraphicsDevice;
	using GraphicsDevice = DX11GraphicsDevice;
#endif

	using Spectrum = glm::vec3;
	using Color3f = glm::vec3;
	using Color4f = glm::vec4;
	using Matrix2f = glm::mat2;
	using Matrix3f = glm::mat3;
	using Matrix4f = glm::mat4;

	template<typename T>
	inline float* crystal_value_ptr(T x) { return glm::value_ptr(x); }

	// Graphics API dependent
	Matrix4f crystal_look_at(const Point3f& eye, const Point3f& center, const Vector3f& up);
	Matrix4f crystal_perspective(Float fovY, Float aspectRatio, Float zNear, Float zFar);

	enum class DataFormat
	{
		Float32,
		Int32,
		UInt32,
		Short16,
		UShort16,
		Byte8,
		SByte8,

		__COUNT
	};

	enum class ComponentFormat
	{
		Single,
		Vector2f,
		Vector3f,
		Vector4f,

		Int,
		Vector2i,
		Vector3i,
		Vector4i,

		Mat2f,
		Mat3f,
		Mat4f,

		__COUNT
	};


	enum class BufferUsage
	{
		Default,
		Immutable,
		CPUWrite,
		CPURead,
		CPURWrite,

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
		R8G8B8A8_UINT, 
		R32G32B32_UINT,

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


	struct VertexBufferDescription
	{
		BufferUsage	Usage;
	};

	struct IndexBufferDescription
	{
		BufferUsage	Usage;
		DataFormat	Format;
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
		VertexLayout() {}
		VertexLayout(const std::vector<ElementDescription>& elementDescs, size_t stride)
			: Elements(elementDescs), Stride(stride)
		{}

		std::vector<ElementDescription> Elements{};
		size_t Stride = 0;
	};

	struct UniformVariable
	{
		std::string		Name;
		ComponentFormat	Format;
	};

	struct UniformVariableCollection
	{
		void Add(const UniformVariable& var) { Variables.push_back(var); }
		void Clear() { Variables.clear(); }
		std::vector<UniformVariable> Variables;
	};

	void InitGraphicsCommons();
	ComponentFormat StringToComponentFormatConvert(const std::string& type);
	DataFormat StringToDataFormatConvert(const std::string& type);
	size_t ComponentFormatToSizeConvert(ComponentFormat format);
}