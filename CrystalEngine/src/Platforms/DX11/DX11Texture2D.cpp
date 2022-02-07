#include "DX11Texture2D.h"
#include "DX11ShaderProgram.h"
#include "DX11GraphicsDevice.h"
#include "DX11VertexShader.h"
#include "DX11FragmentShader.h"

namespace crystal
{
	DX11Texture2D::DX11Texture2D(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11Texture2D> tex2d, ComPtr<ID3D11ShaderResourceView> SRV)
		: m_pGraphicsDevice(graphicsDevice), m_texture2D(tex2d), m_pSRV(SRV)
	{

	}

	DX11Texture2D::~DX11Texture2D()
	{}
}
