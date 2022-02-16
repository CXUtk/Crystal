#include "DX11SamplerState.h"

namespace crystal
{
	DX11SamplerState::DX11SamplerState(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11SamplerState> samplerState)
		: m_pGraphicsDevice(graphicsDevice), m_pSamplerState(samplerState)
	{

	}

	DX11SamplerState::~DX11SamplerState()
	{}

}
