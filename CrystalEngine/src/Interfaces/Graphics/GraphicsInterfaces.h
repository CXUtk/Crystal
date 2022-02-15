#pragma once
#include "GraphicsCommon.h"
#include "IGraphicsDevice.h"
#include "IGraphicsContext.h"
#include "IPipelineResourceObject.h"
#include "IPipelineStateObject.h"

#include "PipelineResources/IShaderResource.h"
#include "PipelineResources/IRenderTarget2D.h"
#include "PipelineResources/ITexture2D.h"
#include "PipelineResources/IShaderProgram.h"
#include "PipelineResources/IFragmentShader.h"
#include "PipelineResources/IVertexShader.h"
#include "PipelineResources/IVertexBuffer.h"
#include "PipelineResources/IIndexBuffer.h"
#include "PipelineResources/ISamplerState.h"
#include "PipelineStates/IBlendState.h"

namespace crystal
{
	class IPlatformProvider
	{
	public:
		virtual ~IPlatformProvider() = 0 {};

		virtual IGameWindow* GetGameWindow() const = 0;
		virtual IFileSystem* GetFileSystem() const = 0;
		virtual IGraphicsDevice* GetGraphicsDevice() const = 0;
		virtual IGraphicsContext* GetGraphicsContext() const = 0;
		virtual void Present() = 0;
	};
}