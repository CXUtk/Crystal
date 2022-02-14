#pragma once
#include "GraphicsCommon.h"

namespace crystal
{
	class IPipelineResourceObject
	{
	public:
		virtual ~IPipelineResourceObject() = 0 {};

		virtual void SetShaderResource(std::shared_ptr<IShaderResource> shaderResource, int slot) = 0;
		virtual void SetSamplerState(std::shared_ptr<ISamplerState> samplerState, int slot) = 0;
		virtual void SetShaderProgram(std::shared_ptr<IShaderProgram> shaderProgram) = 0;

		virtual void SetVertexBuffer(std::shared_ptr<IVertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(std::shared_ptr<IIndexBuffer> vertexBuffer) = 0;
	};
}