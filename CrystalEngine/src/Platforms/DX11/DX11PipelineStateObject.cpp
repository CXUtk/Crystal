#include "DX11PipelineStateObject.h"
#include "DX11GraphicsDevice.h"
#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "DX11VertexShader.h"
#include "DX11FragmentShader.h"
#include "DX11ShaderProgram.h"
#include "DX11Texture2D.h"
#include "DX11SamplerState.h"
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


		m_depthStencilStateDesc.DepthEnable = true;
		m_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		m_depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		m_depthStencilStateDesc.StencilEnable = false;
		m_depthStencilStateDesc.StencilReadMask = 0xFF;
		m_depthStencilStateDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		m_depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		m_depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		m_depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		m_depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
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
		m_rasterStateDesc.CullMode = DX11Common::CullModeConvert(mode);
		m_needsRefreshRasterState = true;
	}

	void DX11PipelineStateObject::SetFillMode(FillMode mode)
	{
		m_rasterStateDesc.FillMode = DX11Common::FillModeConvert(mode);
		m_needsRefreshRasterState = true;
	}

	void DX11PipelineStateObject::SetScissorState(bool enable)
	{
		m_rasterStateDesc.ScissorEnable = enable;
		m_needsRefreshRasterState = true;
	}

	void DX11PipelineStateObject::SetScissorRect(const Bound2i& rect)
	{
		auto minpos = rect.GetMinPos();
		auto maxpos = rect.GetMaxPos();
		auto bufferSize = m_pGraphicsDevice->GetBackBufferSize();
		m_scissorRect.left = minpos.x;
		m_scissorRect.top = bufferSize.y - maxpos.y - 1;
		m_scissorRect.right = maxpos.x;
		m_scissorRect.bottom = bufferSize.y - minpos.y - 1;

		m_needsRefreshScissorRect = true;
	}

	void DX11PipelineStateObject::SetDepthTestState(bool enable)
	{
		m_depthStencilStateDesc.DepthEnable = enable;
		m_needsRefreshDepthStencilState = true;
	}

	void DX11PipelineStateObject::SetStencilTestState(bool enable)
	{
		m_depthStencilStateDesc.StencilEnable = enable;
		m_needsRefreshDepthStencilState = true;
	}

	//void DX11PipelineStateObject::Begin()
	//{
	//	if (m_vertexBuffer)
	//	{
	//		m_vertexBuffer->m_BindToPipeline(0);
	//	}
	//	if (m_indexBuffer)
	//	{
	//		m_indexBuffer->m_BindToPipeline(0);
	//	}
	//	ID3D11ShaderResourceView* viewArray[MAX_SHADER_RESOURCES_COUNT]{};
	//	ID3D11SamplerState* samplerArray[MAX_SHADER_RESOURCES_COUNT]{};

	//	for (int i = 0; i < MAX_SHADER_RESOURCES_COUNT; i++)
	//	{
	//		viewArray[i] = m_SRVSlots[i].Get();
	//		samplerArray[i] = m_samplerStates[i].Get();
	//	}
	//	m_shaderProgram->SetShaderResources(0, MAX_SHADER_RESOURCES_COUNT, viewArray, samplerArray);
	//	m_shaderProgram->Apply();

	//	if (m_needsRefreshRasterState)
	//	{
	//		m_pGraphicsDevice->GetD3DDevice()->CreateRasterizerState(&m_rasterStateDesc,
	//			m_currentRasterizerState.ReleaseAndGetAddressOf());
	//		m_needsRefreshRasterState = false;
	//	}
	//	m_pGraphicsDevice->GetD3DDeviceContext()->RSSetState(m_currentRasterizerState.Get());
	//	m_pGraphicsDevice->GetD3DDeviceContext()->RSSetScissorRects(1, &m_scissorRect);

	//	if (m_needsRefreshDepthStencilState)
	//	{
	//		m_pGraphicsDevice->GetD3DDevice()->CreateDepthStencilState(&m_depthStencilStateDesc,
	//			m_currentDepthStencilState.ReleaseAndGetAddressOf());
	//		m_needsRefreshDepthStencilState = false;
	//	}
	//	m_pGraphicsDevice->GetD3DDeviceContext()->OMSetDepthStencilState(m_currentDepthStencilState.Get(), 0);
	//}

	//void DX11PipelineStateObject::End()
	//{
	//	ID3D11ShaderResourceView* viewArray[1] = { nullptr };
	//	ID3D11SamplerState* samplerArray[1] = { nullptr };
	//	m_shaderProgram->SetShaderResources(0, 1, viewArray, samplerArray);
	//}

	void DX11PipelineStateObject::BindShaderResource(std::shared_ptr<IShaderResource> shaderResource, int index)
	{
		assert(index >= 0 && index < MAX_SHADER_RESOURCES_COUNT);
		m_SRVSlots[index] = shaderResource->GetShaderResourceView();
	}

	void DX11PipelineStateObject::BindSamplerState(std::shared_ptr<SamplerState> samplerState, int index)
	{
		assert(index >= 0 && index < MAX_SHADER_RESOURCES_COUNT);
		m_samplerStates[index] = samplerState->GetDX11Ptr();
	}

	PipelineStateObjectDirtyFlags DX11PipelineStateObject::CheckDirtyFlag(DX11PipelineStateObject* other)
	{
		PipelineStateObjectDirtyFlags flags = static_cast<PipelineStateObjectDirtyFlags>(0);
		
		if (m_vertexBuffer != other->m_vertexBuffer)
		{
			flags = flags | PipelineStateObjectDirtyFlags::CRYSTAL_PSO_VERTEX_BUFFER_DIRTY;
		}

		if (m_indexBuffer != other->m_indexBuffer)
		{
			flags = flags | PipelineStateObjectDirtyFlags::CRYSTAL_PSO_INDEX_BUFFER_DIRTY;
		}

		for (int i = 0; i < MAX_SHADER_RESOURCES_COUNT; i++)
		{
			if (m_SRVSlots[i] != other->m_SRVSlots[i])
			{
				flags = flags | PipelineStateObjectDirtyFlags::CRYSTAL_PSO_SHADER_RESOURCE_DIRTY;
				break;
			}
		}

		if (m_checkRasterizerState(other))
		{
			flags = flags | PipelineStateObjectDirtyFlags::CRYSTAL_PSO_RASTERIZER_STATE_DIRTY;
		}

		if (m_checkDepthStencilState(other))
		{
			flags = flags | PipelineStateObjectDirtyFlags::CRYSTAL_PSO_RASTERIZER_STATE_DIRTY;
		}

		for (int i = 0; i < MAX_SHADER_RESOURCES_COUNT; i++)
		{
			if (m_samplerStates[i] != other->m_samplerStates[i])
			{
				flags = flags | PipelineStateObjectDirtyFlags::CRYSTAL_PSO_SAMPLER_STATE_DIRTY;
				break;
			}
		}
	}

	void DX11PipelineStateObject::Apply(PipelineStateObjectDirtyFlags dirtyFlags)
	{
		auto context = m_pGraphicsDevice->GetD3DDeviceContext();
		if (dirtyFlags & PipelineStateObjectDirtyFlags::CRYSTAL_PSO_VERTEX_BUFFER_DIRTY)
		{
			if (m_vertexBuffer)
			{
				m_vertexBuffer->m_BindToPipeline(0);
			}
		}
		if (dirtyFlags & PipelineStateObjectDirtyFlags::CRYSTAL_PSO_INDEX_BUFFER_DIRTY)
		{
			if (m_indexBuffer)
			{
				m_indexBuffer->m_BindToPipeline(0);
			}
		}
		if (dirtyFlags & PipelineStateObjectDirtyFlags::CRYSTAL_PSO_SHADER_RESOURCE_DIRTY)
		{
			ID3D11ShaderResourceView* viewArray[MAX_SHADER_RESOURCES_COUNT]{};

			for (int i = 0; i < MAX_SHADER_RESOURCES_COUNT; i++)
			{
				viewArray[i] = m_SRVSlots[i].Get();
			}

			context->VSSetShaderResources(0, MAX_SHADER_RESOURCES_COUNT, viewArray);
			context->PSSetShaderResources(0, MAX_SHADER_RESOURCES_COUNT, viewArray);
			context->GSSetShaderResources(0, MAX_SHADER_RESOURCES_COUNT, viewArray);
		}
		if (dirtyFlags & PipelineStateObjectDirtyFlags::CRYSTAL_PSO_RASTERIZER_STATE_DIRTY)
		{
			if (m_needsRefreshRasterState)
			{
				m_pGraphicsDevice->GetD3DDevice()->CreateRasterizerState(&m_rasterStateDesc,
					m_currentRasterizerState.ReleaseAndGetAddressOf());
				m_needsRefreshRasterState = false;
			}
			m_pGraphicsDevice->GetD3DDeviceContext()->RSSetState(m_currentRasterizerState.Get());
			m_pGraphicsDevice->GetD3DDeviceContext()->RSSetScissorRects(1, &m_scissorRect);
		}
		if (dirtyFlags & PipelineStateObjectDirtyFlags::CRYSTAL_PSO_DEPTH_STENCIL_STATE_DIRTY)
		{
			if (m_needsRefreshDepthStencilState)
			{
				m_pGraphicsDevice->GetD3DDevice()->CreateDepthStencilState(&m_depthStencilStateDesc,
					m_currentDepthStencilState.ReleaseAndGetAddressOf());
				m_needsRefreshDepthStencilState = false;
			}
			m_pGraphicsDevice->GetD3DDeviceContext()->OMSetDepthStencilState(m_currentDepthStencilState.Get(), 0);
		}
		if (dirtyFlags & PipelineStateObjectDirtyFlags::CRYSTAL_PSO_SAMPLER_STATE_DIRTY)
		{
			ID3D11SamplerState* samplerArray[MAX_SHADER_RESOURCES_COUNT]{};

			for (int i = 0; i < MAX_SHADER_RESOURCES_COUNT; i++)
			{
				samplerArray[i] = m_samplerStates[i].Get();
			}

			context->VSSetSamplers(0, MAX_SHADER_RESOURCES_COUNT, samplerArray);
			context->PSSetSamplers(0, MAX_SHADER_RESOURCES_COUNT, samplerArray);
			context->GSSetSamplers(0, MAX_SHADER_RESOURCES_COUNT, samplerArray);
		}
	}

	bool DX11PipelineStateObject::m_checkRasterizerState(DX11PipelineStateObject* other)
	{
		return memcmp(&m_rasterStateDesc, &other->m_rasterStateDesc, sizeof(D3D11_RASTERIZER_DESC)) != 0 
			|| m_scissorRect != other->m_scissorRect;
	}
	bool DX11PipelineStateObject::m_checkDepthStencilState(DX11PipelineStateObject* other)
	{
		return memcmp(&m_depthStencilStateDesc, &other->m_depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC)) != 0;
	}
}
