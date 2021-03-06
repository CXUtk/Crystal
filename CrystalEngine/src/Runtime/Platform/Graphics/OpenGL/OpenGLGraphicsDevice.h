#pragma once
#include "OpenGLCommon.h"
#include <Platforms/GLFW/GLFWGameWindow.h>

namespace crystal
{
	class OpenGLGraphicsDevice : public IGraphicsDevice
	{
	public:
		OpenGLGraphicsDevice(const InitArgs& args, GLFWGameWindow* window);
		~OpenGLGraphicsDevice() override;

		virtual void SetPipelineStateObject(std::shared_ptr<PipelineStateObject> pso) override;
		virtual void Clear(ClearOptions options, const Color4f& color, float depth, int stencil) override;
		virtual void Present() override;
		virtual void DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices) override;
		virtual void DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices,
			size_t indexOffset, size_t vertexOffset) override;

		virtual std::shared_ptr<PipelineStateObject> CreatePipelineStateObject() override;
		virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(const VertexBufferDescription& desc, 
			void* src, size_t size) override;
		virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(const IndexBufferDescription& desc, 
			void* src, size_t size) override;
		virtual std::shared_ptr<VertexShader> CreateVertexShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) override;
		virtual std::shared_ptr<FragmentShader> CreateFragmentShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) override;
		virtual std::shared_ptr<ShaderProgram> CreateShaderProgramFromFile(const std::string& path) override;

	private:
		GLFWGameWindow*		m_pWindow;
	};
}