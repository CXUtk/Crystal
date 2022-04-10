#pragma once
#include "GraphicsCommon.h"

namespace crystal
{
	/**
	 * @brief Graphics context interface.
	 * Contains current graphics states and allows user
	 * to manipulate pipeline states and perform draw operations
	*/
	class IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext() = 0 {};

		virtual void Present() = 0;
		virtual void DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices) = 0;
		virtual void DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices,
			size_t indexOffset, size_t vertexOffset) = 0;
		virtual void Clear(ClearOptions options, const Color4f& color, float depth, int stencil) = 0;

		virtual void LoadPipelineState(std::shared_ptr<IPipelineStateObject> pipelineState) = 0;

		virtual void LoadPipelineResources(std::shared_ptr<IPipelineResourceObject> pipelineResource) = 0;
		virtual void UnloadPipelineResources() = 0;

		virtual void PushRenderTarget2D(std::shared_ptr<IRenderTarget2D> renderTarget2D) = 0;
		virtual void PopRenderTarget2D() = 0;

        virtual void SetViewPort(const Viewport& viewport) = 0;

		virtual Vector2i GetCurrentFrameBufferSize() const = 0;
	};
}