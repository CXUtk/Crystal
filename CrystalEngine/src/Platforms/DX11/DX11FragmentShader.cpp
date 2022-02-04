#include "DX11FragmentShader.h"
#include "DX11GraphicsDevice.h"

namespace crystal
{
	DX11FragmentShader::DX11FragmentShader(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11PixelShader> pixelShader)
		: m_pGraphicsDevice(graphicsDevice), m_pPixelShader(pixelShader)
	{}

	DX11FragmentShader::~DX11FragmentShader()
	{}

	void DX11FragmentShader::Bind()
	{
		m_pGraphicsDevice->GetD3DDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	}
}