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

	DX11ShaderProgram::DX11ShaderProgram(DX11GraphicsDevice* graphicsDevice, 
		std::shared_ptr<IVertexShader> vertexShader,
		std::shared_ptr<IFragmentShader> fragmentShader, 
		const UniformVariableCollection& uniforms)
		: m_pGraphicsDevice(graphicsDevice)
	{
		m_shaders.push_back(vertexShader);
		m_shaders.push_back(fragmentShader);

		size_t size = 0;
		for (auto& var : uniforms.Variables)
		{
			m_uniformMap[var.Name] = size;
			size += ComponentFormatToSizeConvert(var.Format);
		}
		// For const buffer the ByteWidth (value = 4) must be a multiple of 16.
		size = (size + 15) / 16 * 16;
		m_pConstantBuffer = m_pGraphicsDevice->CreateBuffer(nullptr, size, 
			BufferUsage::CPUWrite, D3D11_BIND_CONSTANT_BUFFER);
		m_pConstantBufferData = std::make_unique<char[]>(size);
		m_constBufferSize = size;
	}

	DX11ShaderProgram::~DX11ShaderProgram()
	{}

	void DX11ShaderProgram::Apply()
	{
		// Map const buffer data to GPU
		auto context = m_pGraphicsDevice->GetD3DDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, m_constBufferSize, &m_pConstantBufferData[0], m_constBufferSize);
		context->Unmap(m_pConstantBuffer.Get(), 0);
		m_constBufferDirty = false;

		for (auto& shader : m_shaders)
		{
			shader->Apply();
			auto type = shader->GetShaderType();
			if (type == ShaderType::VERTEX_SHADER)
			{
				context->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
			}
			else if (type == ShaderType::FRAGMENT_SHADER)
			{
				context->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
			}
		}
	}
	void DX11ShaderProgram::SetUniform1f(const std::string& name, float value)
	{
		auto iter = m_uniformMap.find(name);
		if (iter == m_uniformMap.end())
		{
			throw std::exception("Cannot find uniform variable");
		}
		memcpy(&m_pConstantBufferData[iter->second], &value, sizeof(float));
		m_constBufferDirty = true;
	}
}
