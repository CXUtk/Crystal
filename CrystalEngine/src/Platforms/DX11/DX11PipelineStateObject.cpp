#include "DX11PipelineStateObject.h"
#include "DX11GraphicsDevice.h"
#include "DX11GraphicsContext.h"

#include "PipelineStates/DX11BlendState.h"
#include "PipelineStates/DX11DepthStencilState.h"
#include "PipelineStates/DX11RasterState.h"

#include "PipelineResources/DX11VertexBuffer.h"
#include "PipelineResources/DX11IndexBuffer.h"
#include "PipelineResources/DX11VertexShader.h"
#include "PipelineResources/DX11FragmentShader.h"
#include "PipelineResources/DX11ShaderProgram.h"
#include "PipelineResources/DX11Texture2D.h"
#include "PipelineResources/DX11SamplerState.h"

#include "d3dUtils.h"
#include "dxTrace.h"
#include <Core/Utils/Misc.h>



namespace crystal
{
	DX11PipelineStateObject::DX11PipelineStateObject(DX11GraphicsDevice* graphicsDevice,
		DX11GraphicsContext* graphicsContext)
		: m_pGraphicsDevice(graphicsDevice), m_pGraphicsContext(graphicsContext)
	{}

	DX11PipelineStateObject::~DX11PipelineStateObject()
	{}

	void DX11PipelineStateObject::SetRasterState(std::shared_ptr<IRasterState> rasterState)
	{
		m_pRasterizerState = std::dynamic_pointer_cast<DX11RasterState>(rasterState);
	}

	void DX11PipelineStateObject::SetDepthStencilState(std::shared_ptr<IDepthStencilState> depthStencilState)
	{
		m_pDepthStencilState = std::dynamic_pointer_cast<DX11DepthStencilState>(depthStencilState);
	}

	void DX11PipelineStateObject::SetBlendState(std::shared_ptr<IBlendState> blendState)
	{
		m_pBlendState = std::dynamic_pointer_cast<DX11BlendState>(blendState);
	}

	void DX11PipelineStateObject::Load()
	{
		auto context = m_pGraphicsContext->GetD3DContext();
		if (!m_pRasterizerState || !m_pDepthStencilState || !m_pBlendState)
		{
			throw std::logic_error("Pipeline states are not specified");
		}
		m_pRasterizerState->Load(m_pGraphicsContext);
		m_pDepthStencilState->Load(m_pGraphicsContext);
		m_pBlendState->Load(m_pGraphicsContext);
	}

	void DX11PipelineStateObject::Unload()
	{
		m_pRasterizerState->Unload(m_pGraphicsContext);
		m_pDepthStencilState->Unload(m_pGraphicsContext);
		m_pBlendState->Unload(m_pGraphicsContext);
	}
}
