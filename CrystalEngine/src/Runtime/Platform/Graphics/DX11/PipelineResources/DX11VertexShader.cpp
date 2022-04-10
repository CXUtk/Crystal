#include "DX11VertexShader.h"
#include "../DX11GraphicsDevice.h"
#include "../d3dUtils.h"
#include "../dxTrace.h"
#include <Core/Utils/Misc.h>

namespace crystal
{
	DX11VertexShader::DX11VertexShader(DX11GraphicsDevice* graphicsDevice,
		ComPtr<ID3D11VertexShader> vertexShader)
		: m_pGraphicsDevice(graphicsDevice), m_pVertexShader(vertexShader)
	{
		d3dUtils::D3D11SetDebugObjectName(m_pVertexShader.Get(), "Vertex Shader");
	}

	DX11VertexShader::~DX11VertexShader()
	{}

	void DX11VertexShader::SetToCurrentContext(ID3D11DeviceContext * context)
	{
		context->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	}
}
