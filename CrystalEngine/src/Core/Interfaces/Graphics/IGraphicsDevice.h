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
		virtual void PushRenderTarget2D(std::shared_ptr<IRenderTarget2D> renderTarget2D) = 0;
		virtual void PopRenderTarget2D() = 0;
		virtual void PushPipelineStateObject(std::shared_ptr<IPipelineStateObject> pso) = 0;
		virtual void PopPipelineStateObject() = 0;

		virtual std::shared_ptr<IPipelineStateObject> CreatePipelineStateObject() = 0;
		virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const VertexBufferDescription& desc, void* src, size_t size) = 0;
		virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer(const IndexBufferDescription& desc, void* src, size_t size) = 0;
		virtual std::shared_ptr<IVertexShader> CreateVertexShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) = 0;
		virtual std::shared_ptr<IFragmentShader> CreateFragmentShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) = 0;
		virtual std::shared_ptr<IShaderProgram> CreateShaderProgramFromFile(const std::string& path) = 0;
		virtual std::shared_ptr<ITexture2D> CreateTexture2DFromFile(const std::string& path, const Texture2DDescription& texDesc) = 0;
		virtual std::shared_ptr<ITexture2D> CreateTexture2DFromMemory(const uint8_t* src, size_t size, const Texture2DDescription& texDesc) = 0;
		virtual std::shared_ptr<IRenderTarget2D> CreateRenderTarget2D(const RenderTarget2DDescription& desc) = 0;

		virtual std::shared_ptr<ISamplerState> GetCommonSamplerState(SamplerStates state) = 0;
		virtual std::shared_ptr<IBlendState> GetCommonBlengState(BlendStates state) = 0;
	};
}