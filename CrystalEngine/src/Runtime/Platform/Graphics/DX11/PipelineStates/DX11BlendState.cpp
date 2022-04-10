#include "DX11BlendState.h"
#include "../DX11GraphicsContext.h"
#include "../DX11GraphicsDevice.h"
#include "../dxTrace.h"

namespace crystal
{
	DX11BlendState::DX11BlendState(DX11GraphicsDevice* graphicsDevice, 
		const BlendStateDescription& blendStateDesc)
		: m_pGraphicsDevice(graphicsDevice)
	{
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = blendStateDesc.EnableBlending;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = DX11Common::BlendFactorConvert(blendStateDesc.SrcBlend);
		blendDesc.RenderTarget[0].DestBlend = DX11Common::BlendFactorConvert(blendStateDesc.DestBlend);
		blendDesc.RenderTarget[0].BlendOp = DX11Common::BlendOpConvert(blendStateDesc.BlendOp);
		blendDesc.RenderTarget[0].SrcBlendAlpha = DX11Common::BlendFactorConvert(blendStateDesc.SrcBlendAlpha);
		blendDesc.RenderTarget[0].DestBlendAlpha = DX11Common::BlendFactorConvert(blendStateDesc.DestBlendAlpha);
		blendDesc.RenderTarget[0].BlendOpAlpha = DX11Common::BlendOpConvert(blendStateDesc.BlendOpAlpha);
		blendDesc.RenderTarget[0].RenderTargetWriteMask = blendStateDesc.RenderTargetWriteMask;

		HR(m_pGraphicsDevice->GetD3DDevice()->CreateBlendState(&blendDesc, m_pBlendState.GetAddressOf()));
	}

	DX11BlendState::~DX11BlendState()
	{
		
	}

	void DX11BlendState::Load(DX11GraphicsContext * context)
	{
		float blendFactors[4] = { 0.f, 0.f, 0.f, 0.f };
		context->GetD3DContext()->OMSetBlendState(m_pBlendState.Get(), blendFactors, -1);
	}

	void DX11BlendState::Unload(DX11GraphicsContext* context)
	{
		
	}

}
