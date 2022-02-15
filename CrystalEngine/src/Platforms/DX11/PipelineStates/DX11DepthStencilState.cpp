#include "DX11DepthStencilState.h"

#include "../DX11GraphicsContext.h"
#include "../DX11GraphicsDevice.h"
#include "../dxTrace.h"
namespace crystal
{
	DX11DepthStencilState::DX11DepthStencilState(DX11GraphicsDevice* graphicsDevice,
		const DepthStencilStateDescription& DSDesc)
		: m_pGraphicsDevice(graphicsDevice)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.DepthEnable = DSDesc.EnableDepthTest;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		depthStencilDesc.StencilEnable = DSDesc.EnableStencilTest;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


		HR(m_pGraphicsDevice->GetD3DDevice()->CreateDepthStencilState(&depthStencilDesc,
			m_pDepthStencilState.GetAddressOf()));
	}

	DX11DepthStencilState::~DX11DepthStencilState()
	{}

	void DX11DepthStencilState::Load(DX11GraphicsContext* context)
	{
		context->GetD3DContext()->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
	}

	void DX11DepthStencilState::Unload(DX11GraphicsContext* context)
	{
	}
}