#pragma once
#include "GraphicsCommon.h"

namespace crystal
{
	/**
	 * @brief Graphics device interface. 
	 * Includes platform independent operations to create and draw rendering objects.
	*/
	class IGraphicsDevice
	{
	public:
		virtual ~IGraphicsDevice() = 0 {};

		virtual std::shared_ptr<IGraphicsContext> GetContext() = 0;

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
		virtual std::shared_ptr<IRasterState> CreateRasterState(const RasterStateDescription& rasterDesc) = 0;
		virtual std::shared_ptr<IBlendState> CreateBlendState(const BlendStateDescription& blendDesc) = 0;
		virtual std::shared_ptr<IDepthStencilState> CreateDepthStencilState(const DepthStencilStateDescription& DSSDesc) = 0;

		virtual std::shared_ptr<IPipelineStateObject> CreatePipelineStateObject() = 0;
		virtual std::shared_ptr<IPipelineResourceObject> CreatePipelineResourceObject() = 0;
		virtual std::shared_ptr<ISamplerState> GetCommonSamplerState(SamplerStates state) = 0;
		virtual std::shared_ptr<IBlendState> GetCommonBlendState(BlendStates state) = 0;
	};
}