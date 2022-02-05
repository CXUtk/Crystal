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
		virtual void SetPipelineStateObject(std::shared_ptr<PipelineStateObject> pso) = 0;
		
		virtual std::shared_ptr<PipelineStateObject> CreatePipelineStateObject() = 0;
		virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(const VertexBufferDescription& desc, void* src, size_t size) = 0;
		virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(const IndexBufferDescription& desc, void* src, size_t size) = 0;
		virtual std::shared_ptr<VertexShader> CreateVertexShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) = 0;
		virtual std::shared_ptr<FragmentShader> CreateFragmentShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) = 0;
		virtual std::shared_ptr<ShaderProgram> CreateShaderProgramFromFile(const std::string& path) = 0;

		
	};

	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = 0 {};

		virtual void BindVertexLayout(const VertexLayout& layout) = 0;
	};


	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = 0 {};
	};

	class IShaderProgram
	{
	public:
		virtual ~IShaderProgram() = 0 {};

		virtual void Apply() = 0;
		virtual void SetUniform1f(const std::string& name, float value) = 0;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) = 0;
	};

	class IPipelineStateObject
	{
	public:
		virtual ~IPipelineStateObject() = 0 {};

		virtual void BindVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
		virtual void BindIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) = 0;
		virtual void BindShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram) = 0;

		virtual CullingMode GetCullMode() const = 0;
		virtual FillMode GetFillMode() const = 0;

		virtual void SetCullMode(CullingMode mode) = 0;
		virtual void SetFillMode(FillMode mode) = 0;

		virtual void SetScissorState(bool enable) = 0;
		virtual void SetScissorRect(const Bound2i& rect) = 0;

		virtual void Apply() = 0;
	};

	class IPlatformProvider
	{
	public:
		virtual ~IPlatformProvider() = 0 {};

		virtual IGameWindow* GetGameWindow() const = 0;
		virtual IFileSystem* GetFileSystem() const = 0;
		virtual IGraphicsDevice* GetGraphicsDevice() const = 0;
	};
}