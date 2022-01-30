#include "DX11Shaders.h"
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

	void DX11VertexShader::Apply()
	{
		m_pGraphicsDevice->GetD3DDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	}

	DX11FragmentShader::DX11FragmentShader(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11PixelShader> pixelShader)
		: m_pGraphicsDevice(graphicsDevice), m_pPixelShader(pixelShader)
	{}

	DX11FragmentShader::~DX11FragmentShader()
	{}

	void DX11FragmentShader::Apply()
	{
		m_pGraphicsDevice->GetD3DDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	}

	DX11ShaderProgram::DX11ShaderProgram(std::shared_ptr<IVertexShader> vertexShader,
		std::shared_ptr<IFragmentShader> fragmentShader)
	{
		m_shaders.push_back(vertexShader);
		m_shaders.push_back(fragmentShader);
	}

	DX11ShaderProgram::~DX11ShaderProgram()
	{}

	void DX11ShaderProgram::Apply()
	{
		for (auto& shader : m_shaders)
		{
			shader->Apply();
		}
	}
}
