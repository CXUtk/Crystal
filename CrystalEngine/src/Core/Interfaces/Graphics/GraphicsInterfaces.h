#pragma once
#include "GraphicsCommon.h"

namespace crystal
{
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