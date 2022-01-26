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

		/**
		 * @brief Set vertex attributes for current binding vertex array object
		 * @param layout The vertex layout used for this vertex array buffer
		*/
		virtual void SetupVertexAttributes(const VertexLayout& layout) = 0;

		/**
		 * @brief Create a new vertex array object
		 * @return The handle to that vertex array
		*/
		virtual IVertexBufferHandle CreateVertexArray() = 0;

		/**
		 * @brief Create a new buffer object
		 * @return The handle to that vertex buffer
		*/
		virtual IVertexBufferHandle CreateBuffer() = 0;

		/**
		 * @brief Create multiple vertex arrays
		 * @param size The number of vertex arrays to create
		 * @param hOut Handler array to store those objects
		*/
		virtual void CreateVertexArrays(int size, IVertexBufferHandle* hOut) = 0;

		/**
		 * @brief Create multiple vertex buffers
		 * @param size The number of vertex buffers to create
		 * @param hOut Handler array to store those objects
		*/
		virtual void CreateBuffers(int size, IVertexBufferHandle* hOut) = 0;

		/**
		 * @brief Bind the buffer object and set its size
		 * @param type Type of the buffer object
		 * @param handle Handle to the buffer object
		 * @param size Size of the buffer
		 * @param data Data source to initialize this buffer, nullptr if you don't want to initialize
		 * @param bufferHint Hint to this buffer. @see BufferHint
		*/
		virtual void SetBufferData(BufferType type, IVertexBufferHandle handle,
			size_t size, const void* data, BufferHint bufferHint) = 0;

		/**
		 * @brief Subsitute the data in given buffer object
		 * @param type Type of the buffer object
		 * @param handle Handle to the buffer object
		 * @param offset Copy starting offset of this action
		 * @param size Size of the data
		 * @param data Data source that will be copied to this buffer
		*/
		virtual void ChangeBufferData(BufferType type, IVertexBufferHandle handle,
			size_t offset, size_t size, const void* data) = 0;

		/**
		 * @brief Bind the buffer to current context
		 * @param type Type of the buffer
		 * @param handle Handle of the buffer object
		*/
		virtual void BindBuffer(BufferType type, IVertexBufferHandle handle) = 0;

		/**
		 * @brief Bind the vertex array object to current context
		 * @param handle Handle of the vertex array object
		*/
		virtual void BindVertexArray(IVertexBufferHandle handle) = 0;

		/**
		 * @brief Unbind current vertex array object from context
		*/
		virtual void UnbindVertexArray() = 0;

		/**
		 * @brief Bind a texture 2d object to current context
		 * @param slot Texture slot in graphics device
		 * @param textureHandle
		*/
		virtual void BindTexture2DSlot(int slot, const Texture2D* texture) = 0;

		/**
		 * @brief Get maximum texture slot number in this graphics device
		 * @return
		*/
		virtual int GetMaxTextureSlots() = 0;

		/**
		 * @brief Draw primitives using indexed buffer from current context
		 * @param type Type of the primitives
		 * @param count Number of such primitives
		 * @param dataType Data type of indicies
		 * @param offset Starting offset of the drawing buffer
		*/
		virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, EngineDataType dataType, size_t offset) = 0;

		virtual void DrawPrimitives(PrimitiveType type, size_t count, size_t offset) = 0;

		virtual void SwitchRenderTarget(const RenderTarget2D* renderTarget) = 0;
		virtual void SetViewPort(int x, int y, int width, int height) = 0;
		virtual void Clear(const glm::vec4& color) = 0;

		virtual void UseShader(const ShaderProgram* shader) = 0;

		virtual void SetBlendingMode(BlendingMode mode) = 0;
		virtual void SetDepthTestingMode(DepthTestingMode mode, DepthTestingFunction func) = 0;
		virtual void SetCullingMode(CullingMode mode) = 0;
		virtual void SetPolygonMode(PolygonMode mode) = 0;

		virtual byte_color ReadPixelFromTexture(const Texture2D* texture, int x, int y) = 0;
	};

}