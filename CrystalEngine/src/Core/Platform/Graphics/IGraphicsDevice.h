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
		
		virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const VertexBufferDescription& desc, void* src, size_t size) = 0;
		virtual std::shared_ptr<IVertexShader> CreateVertexShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) = 0;
		virtual std::shared_ptr<IFragmentShader> CreateFragmentShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) = 0;
		virtual std::shared_ptr<IShaderProgram> CreateShaderProgram(std::shared_ptr<IVertexShader> vertexShader,
			std::shared_ptr<IFragmentShader> fragmentShader) = 0;
	};



	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = 0 {};

		virtual void BindVertexLayout(const VertexLayout& layout) = 0;
		virtual void Bind(size_t offset) = 0;
	};


	class IShaderProgram
	{
	public:
		virtual ~IShaderProgram() = 0 {};

		virtual void Apply() = 0;
	};
}