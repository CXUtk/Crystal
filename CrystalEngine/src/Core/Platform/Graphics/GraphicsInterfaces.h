#pragma once
#include "GraphicsCommon.h"

namespace crystal
{

	/**
	 * @brief Graphics device interface. Includes platform independent operations to create and draw rendering objects.
	*/
	class IGraphicsDevice
	{
	public:
		virtual ~IGraphicsDevice() = 0 {};

		virtual void Clear(ClearOptions options, const Color4f& color, float depth, int stencil) = 0;
		virtual void Present() = 0;
		virtual void DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices) = 0;
		virtual void DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices,
			size_t indexOffset, size_t vertexOffset) = 0;
		virtual void PushRenderTarget2D(std::shared_ptr<RenderTarget2D> renderTarget2D) = 0;
		virtual void PopRenderTarget2D() = 0;
		virtual void PushPipelineStateObject(std::shared_ptr<PipelineStateObject> pso) = 0;
		virtual void PopPipelineStateObject() = 0;
		virtual void BindShaderResource(std::shared_ptr<IShaderResource> shaderResource, int index) = 0;
		virtual void BindSamplerState(std::shared_ptr<SamplerState> texture, int index) = 0;

		virtual std::shared_ptr<PipelineStateObject> CreatePipelineStateObject() = 0;
		virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(const VertexBufferDescription& desc, void* src, size_t size) = 0;
		virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(const IndexBufferDescription& desc, void* src, size_t size) = 0;
		virtual std::shared_ptr<VertexShader> CreateVertexShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) = 0;
		virtual std::shared_ptr<FragmentShader> CreateFragmentShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) = 0;
		virtual std::shared_ptr<ShaderProgram> CreateShaderProgramFromFile(const std::string& path) = 0;
		virtual std::shared_ptr<Texture2D> CreateTexture2DFromFile(const std::string& path, const Texture2DDescription& texDesc) = 0;
		virtual std::shared_ptr<Texture2D> CreateTexture2DFromMemory(const uint8_t* src, size_t size, const Texture2DDescription& texDesc) = 0;
		virtual std::shared_ptr<RenderTarget2D> CreateRenderTarget2D(const RenderTarget2DDescription& desc) = 0;
	
		virtual std::shared_ptr<SamplerState> GetCommonSamplerState(SamplerStates state) = 0;
	};

	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = 0 {};

		virtual void BindVertexLayout(const VertexLayout& layout) = 0;
		virtual void ChangeBufferContent(const void* src, size_t size, size_t offset) = 0;
	};


	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = 0 {};
	};

	class ITexture2D
	{
	public:
		virtual ~ITexture2D() = 0 {};
	};

	class IShaderProgram
	{
	public:
		virtual ~IShaderProgram() = 0 {};

		virtual void Apply() = 0;
		virtual void SetUniform1f(const std::string& name, float value) = 0;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) = 0;
	};


	class ISamplerState
	{
	public:
		virtual ~ISamplerState() = 0 {};
	};

	class IPipelineStateObject
	{
	public:
		virtual ~IPipelineStateObject() = 0 {};

		virtual void BindVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
		virtual void BindIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) = 0;

		virtual CullingMode GetCullMode() const = 0;
		virtual FillMode GetFillMode() const = 0;

		virtual void SetCullMode(CullingMode mode) = 0;
		virtual void SetFillMode(FillMode mode) = 0;

		virtual void SetScissorState(bool enable) = 0;
		virtual void SetScissorRect(const Bound2i& rect) = 0;

		virtual void SetDepthTestState(bool enable) = 0;
		virtual void SetStencilTestState(bool enable) = 0;
	};

	class IRenderTarget2D
	{
	public:
		virtual ~IRenderTarget2D() = 0 {};
	};

	class IPlatformProvider
	{
	public:
		virtual ~IPlatformProvider() = 0 {};

		virtual IGameWindow* GetGameWindow() const = 0;
		virtual IFileSystem* GetFileSystem() const = 0;
		virtual GraphicsDevice* GetGraphicsDevice() const = 0;
		virtual void Present() = 0;
	};

	class IShader
	{
	public:
		virtual ~IShader() = 0 {};
	};


	class IVertexShader : public IShader
	{
	public:
		virtual ~IVertexShader() = 0 {};
	};

	class IFragmentShader : public IShader
	{
	public:
		virtual ~IFragmentShader() = 0 {};
	};
}