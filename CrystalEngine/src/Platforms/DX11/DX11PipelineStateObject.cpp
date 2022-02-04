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
		m_rasterStateDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		m_rasterStateDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		m_rasterStateDesc.FrontCounterClockwise = false;
		m_rasterStateDesc.DepthBias = 0;
		m_rasterStateDesc.DepthBiasClamp = 0.0f;
		m_rasterStateDesc.SlopeScaledDepthBias = 0.0f;
		m_rasterStateDesc.DepthClipEnable = true;
		m_rasterStateDesc.ScissorEnable = false;
		m_rasterStateDesc.MultisampleEnable = false;
		m_rasterStateDesc.AntialiasedLineEnable = false;
		graphicsDevice->GetD3DDevice()->CreateRasterizerState(&m_rasterStateDesc, 
			m_currentRasterizerState.GetAddressOf());
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
