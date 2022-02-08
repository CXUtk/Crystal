#include "DX11FragmentShader.h"
#include "DX11GraphicsDevice.h"
#include "d3dUtils.h"

namespace crystal
{
	DX11FragmentShader::DX11FragmentShader(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11PixelShader> pixelShader)
		: m_pGraphicsDevice(graphicsDevice), m_pPixelShader(pixelShader)
	{
		d3dUtils::D3D11SetDebugObjectName(m_pPixelShader.Get(), "Pixel Shader");
	}

	DX11FragmentShader::~DX11FragmentShader()
	{}

	void DX11FragmentShader::Bind()
	{
		m_pGraphicsDevice->GetD3DDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	}
}