#include "DX11SamplerState.h"
#include "DX11Texture2D.h"
#include "DX11ShaderProgram.h"
#include "DX11GraphicsDevice.h"
#include "DX11VertexShader.h"
#include "DX11FragmentShader.h"
#include "dxTrace.h"
#include "d3dUtils.h"

namespace crystal
{
	static void initStatics()
	{
		
	}

	DX11SamplerState::DX11SamplerState(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11SamplerState> samplerState)
		: m_pGraphicsDevice(graphicsDevice), m_samplerState(samplerState)
	{

	}

	DX11SamplerState::~DX11SamplerState()
	{}

}
