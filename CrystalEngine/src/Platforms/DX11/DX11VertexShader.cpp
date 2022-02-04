#include "DX11VertexShader.h"
#include "DX11GraphicsDevice.h"
#include "d3dUtils.h"
#include "dxTrace.h"
#include <Core/Utils/Misc.h>

namespace crystal
{
	DX11VertexShader::DX11VertexShader(DX11GraphicsDevice* graphicsDevice,
		ComPtr<ID3D11VertexShader> vertexShader)
		: m_pGraphicsDevice(graphicsDevice), m_pVertexShader(vertexShader)
	{
		
	}

	DX11VertexShader::~DX11VertexShader()
	{}

	void DX11VertexShader::Bind()
	{
		m_pGraphicsDevice->GetD3DDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	}
}