#include "DX11PipelineResourceObject.h"
#include "DX11GraphicsContext.h"

#include "PipelineResources/DX11VertexBuffer.h"
#include "PipelineResources/DX11IndexBuffer.h"
#include "PipelineResources/DX11ShaderProgram.h"
#include "PipelineResources/DX11SamplerState.h"

namespace crystal
{
	DX11PipelineResourceObject::DX11PipelineResourceObject(DX11GraphicsDevice* graphicsDevice, 
		DX11GraphicsContext* graphicsContext)
		: m_pGraphicsDevice(graphicsDevice), m_pGraphicsContext(graphicsContext)
	{
		
	}

	DX11PipelineResourceObject::~DX11PipelineResourceObject()
	{}

	void DX11PipelineResourceObject::SetShaderResource(std::shared_ptr<IShaderResource> shaderResource, int slot)
	{
		assert(slot >= 0 && slot < MAX_SHADER_RESOURCES_SLOTS);
		m_SRVSlots[slot] = shaderResource;
	}

	void DX11PipelineResourceObject::SetSamplerState(std::shared_ptr<ISamplerState> samplerState, int slot)
	{
		assert(slot >= 0 && slot < MAX_SHADER_RESOURCES_SLOTS);
		m_SamplerSlots[slot] = std::dynamic_pointer_cast<DX11SamplerState>(samplerState);
	}

	void DX11PipelineResourceObject::SetShaderProgram(std::shared_ptr<IShaderProgram> shaderProgram)
	{
		m_pShaderProgram = std::dynamic_pointer_cast<DX11ShaderProgram>(shaderProgram);
	}

	void DX11PipelineResourceObject::SetVertexBuffer(std::shared_ptr<IVertexBuffer> vertexBuffer)
	{
		m_pVertexBuffer = std::dynamic_pointer_cast<DX11VertexBuffer>(vertexBuffer);
	}

	void DX11PipelineResourceObject::SetIndexBuffer(std::shared_ptr<IIndexBuffer> indexBuffer)
	{
		m_pIndexBuffer = std::dynamic_pointer_cast<DX11IndexBuffer>(indexBuffer);
	}

	void DX11PipelineResourceObject::Load()
	{
		auto context = m_pGraphicsContext->GetD3DContext();
		if (m_pVertexBuffer)
		{
			m_pVertexBuffer->SetToCurrentContext(context, 0);
		}

		if (m_pIndexBuffer)
		{
			m_pIndexBuffer->SetToCurrentContext(context, 0);
		}

		m_pShaderProgram->SetToCurrentContext(context);

		auto shaderMask = m_pShaderProgram->GetShaderMask();
		ID3D11ShaderResourceView* SRVs[MAX_SHADER_RESOURCES_SLOTS] = {};
		ID3D11SamplerState* samplers[MAX_SHADER_RESOURCES_SLOTS] = {};
		for (int i = 0; i < MAX_SHADER_RESOURCES_SLOTS; i++)
		{
			m_SRVSlots[i]->GetShaderResourceHandle((void**)&SRVs[i]);
		}
		for (int i = 0; i < MAX_SHADER_RESOURCES_SLOTS; i++)
		{
			samplers[i] = m_SamplerSlots[i]->GetDX11SamplerState().Get();
		}

		if (shaderMask & ShaderMask::CRYSTAL_SHADERMASK_VERTEX_SHADER)
		{
			context->VSSetShaderResources(0, MAX_SHADER_RESOURCES_SLOTS, &SRVs[0]);
			context->VSSetSamplers(0, MAX_SHADER_RESOURCES_SLOTS, &samplers[0]);
		}
		if (shaderMask & ShaderMask::CRYSTAL_SHADERMASK_FRAGMENT_SHADER)
		{
			context->PSSetShaderResources(0, MAX_SHADER_RESOURCES_SLOTS, &SRVs[0]);
			context->PSSetSamplers(0, MAX_SHADER_RESOURCES_SLOTS, &samplers[0]);
		}
	}

	void DX11PipelineResourceObject::Unload()
	{}
}
