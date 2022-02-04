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
	CullingMode DX11PipelineStateObject::GetCullMode() const
	{
		if (m_rasterStateDesc.CullMode == D3D11_CULL_MODE::D3D11_CULL_NONE)
		{
			return CullingMode::None;
		}
		if (m_rasterStateDesc.FrontCounterClockwise)
		{
			return (m_rasterStateDesc.CullMode == D3D11_CULL_MODE::D3D11_CULL_FRONT) ?
				CullingMode::CullCCW : CullingMode::CullCW;
		}
		else
		{
			return (m_rasterStateDesc.CullMode == D3D11_CULL_MODE::D3D11_CULL_FRONT) ?
				CullingMode::CullCW : CullingMode::CullCCW;
		}
	}
	FillMode DX11PipelineStateObject::GetFillMode() const
	{
		return (m_rasterStateDesc.FillMode == D3D11_FILL_MODE::D3D11_FILL_SOLID)
			? FillMode::SOLID : FillMode::WIREFRAME;
	}

	void DX11PipelineStateObject::SetCullMode(CullingMode mode)
	{
		m_rasterStateDesc.CullMode = CullModeConvert(mode);
		m_needsRefreshRasterState = true;
	}

	void DX11PipelineStateObject::SetFillMode(FillMode mode)
	{
		m_rasterStateDesc.FillMode = FillModeConvert(mode);
		m_needsRefreshRasterState = true;
	}

	ID3D11RasterizerState* DX11PipelineStateObject::GetRasterizerState()
	{
		if (m_needsRefreshRasterState)
		{
			m_pGraphicsDevice->GetD3DDevice()->CreateRasterizerState(&m_rasterStateDesc,
				m_currentRasterizerState.GetAddressOf());
			m_needsRefreshRasterState = false;
		}
		return m_currentRasterizerState.Get();
	}
}
