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
		depthStencilDesc.DepthFunc = DX11Common::ComparisonFunctionConvert(DSDesc.DepthFunction);
		depthStencilDesc.DepthWriteMask = DSDesc.EnableDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL: D3D11_DEPTH_WRITE_MASK_ZERO;

		depthStencilDesc.StencilEnable = DSDesc.EnableStencilTest;
		depthStencilDesc.StencilReadMask = DSDesc.StencilReadMask;
		depthStencilDesc.StencilWriteMask = DSDesc.StencilWriteMask;

		// Stencil operations if pixel is front-facing
		depthStencilDesc.FrontFace.StencilFailOp = DX11Common::StencilOperationConvert(DSDesc.StencilFailedOp);
		depthStencilDesc.FrontFace.StencilDepthFailOp = DX11Common::StencilOperationConvert(DSDesc.DepthFailedOp);
		depthStencilDesc.FrontFace.StencilPassOp = DX11Common::StencilOperationConvert(DSDesc.StencilPassedOp);
		depthStencilDesc.FrontFace.StencilFunc = DX11Common::ComparisonFunctionConvert(DSDesc.StencilFunction);

		// Stencil operations if pixel is back-facing
		depthStencilDesc.BackFace.StencilFailOp = DX11Common::StencilOperationConvert(DSDesc.StencilFailedOp);
		depthStencilDesc.BackFace.StencilDepthFailOp = DX11Common::StencilOperationConvert(DSDesc.DepthFailedOp);
		depthStencilDesc.BackFace.StencilPassOp = DX11Common::StencilOperationConvert(DSDesc.StencilPassedOp);
		depthStencilDesc.BackFace.StencilFunc = DX11Common::ComparisonFunctionConvert(DSDesc.StencilFunction);


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