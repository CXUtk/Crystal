#include "DX11RasterState.h"

#include "../DX11GraphicsContext.h"
#include "../DX11GraphicsDevice.h"
#include "../dxTrace.h"

namespace crystal
{

	DX11RasterState::DX11RasterState(DX11GraphicsDevice* graphicsDevice,
		const RasterStateDescription& rasterDesc)
		: m_pGraphicsDevice(graphicsDevice)
	{
		D3D11_RASTERIZER_DESC rasterStateDesc;
		rasterStateDesc.FillMode = DX11Common::FillModeConvert(rasterDesc.FillMode);
		rasterStateDesc.CullMode = DX11Common::CullModeConvert(rasterDesc.CullMode);
		rasterStateDesc.FrontCounterClockwise = false;
		rasterStateDesc.DepthBias = 0;
		rasterStateDesc.DepthBiasClamp = 0.0f;
		rasterStateDesc.SlopeScaledDepthBias = 0.0f;
		rasterStateDesc.DepthClipEnable = true;
		rasterStateDesc.ScissorEnable = rasterDesc.EnableScissorTest;
		rasterStateDesc.MultisampleEnable = false;
		rasterStateDesc.AntialiasedLineEnable = rasterDesc.EnableAntialiasedLine;

		m_enableScissor = rasterDesc.EnableScissorTest;
		m_scissorBound = rasterDesc.ScissorBound;

		HR(graphicsDevice->GetD3DDevice()->CreateRasterizerState(&rasterStateDesc, 
			m_pRasterState.GetAddressOf()));

		if (rasterDesc.Viewport)
		{
			m_pViewport = std::make_unique<D3D11_VIEWPORT>();

			auto minPos = rasterDesc.Viewport->GetMinPos();
			auto maxPos = rasterDesc.Viewport->GetMaxPos();
			m_pViewport->TopLeftX = minPos.x;
			m_pViewport->TopLeftY = minPos.y;
			m_pViewport->Width = maxPos.x;
			m_pViewport->TopLeftY = maxPos.y;
			m_pViewport->MinDepth = minPos.z;
			m_pViewport->MaxDepth = maxPos.z;
		}
	}

	DX11RasterState::~DX11RasterState()
	{}

	void DX11RasterState::Load(DX11GraphicsContext* context)
	{
		auto dx11Context = context->GetD3DContext();
		if (m_pViewport)
		{
			dx11Context->RSSetViewports(1, m_pViewport.get());
		}
		if (m_enableScissor)
		{
			auto minpos = m_scissorBound.GetMinPos();
			auto maxpos = m_scissorBound.GetMaxPos();
			auto bufferSize = context->GetBackBufferSize();

			RECT scissorRect;
			scissorRect.left = minpos.x;
			scissorRect.top = bufferSize.y - maxpos.y - 1;
			scissorRect.right = maxpos.x;
			scissorRect.bottom = bufferSize.y - minpos.y - 1;
			dx11Context->RSSetScissorRects(1, &scissorRect);
		}
		dx11Context->RSSetState(m_pRasterState.Get());
	}

	void DX11RasterState::Unload(DX11GraphicsContext* context)
	{
		auto dx11Context = context->GetD3DContext();
		if (m_pViewport)
		{
			context->ReturnLastViewports();
		}
		if (m_enableScissor)
		{
			dx11Context->RSSetScissorRects(1, nullptr);
		}
	}
}