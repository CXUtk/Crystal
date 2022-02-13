#include "DX11BlendState.h"
#include <Platforms/DX11/DX11GraphicsDevice.h>
#include <Platforms/DX11/dxTrace.h>

namespace crystal
{
	DX11BlendState::DX11BlendState(DX11GraphicsDevice* graphicsDevice, 
		ComPtr<ID3D11BlendState> blendState)
		: m_pGraphicsDevice(graphicsDevice), m_blendState(blendState)
	{}

	DX11BlendState::~DX11BlendState()
	{}

	void DX11BlendState::Apply()
	{
		auto context = m_pGraphicsDevice->GetD3DDeviceContext();
		float blendFactors[4] = { 0.f, 0.f, 0.f, 0.f };
		context->OMSetBlendState(m_blendState.Get(), blendFactors, -1);
	}
}
