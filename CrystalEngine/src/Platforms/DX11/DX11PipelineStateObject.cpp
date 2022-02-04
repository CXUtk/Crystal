#include "DX11PipelineStateObject.h"
#include "DX11GraphicsDevice.h"
#include "d3dUtils.h"
#include "dxTrace.h"
#include <Core/Utils/Misc.h>

namespace crystal
{
	DX11PipelineStateObject::DX11PipelineStateObject(DX11GraphicsDevice* graphicsDevice)
		: m_pGraphicsDevice(graphicsDevice)
	{
		
	}

	DX11PipelineStateObject::~DX11PipelineStateObject()
	{}

	void DX11PipelineStateObject::BindVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		m_vertexBuffer = vertexBuffer;
	}

	void DX11PipelineStateObject::BindIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
	{
		m_indexBuffer = indexBuffer;
	}

	void DX11PipelineStateObject::BindShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram)
	{
		m_shaderProgram = shaderProgram;
	}
}
