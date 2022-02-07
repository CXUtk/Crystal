#include "DX11ShaderProgram.h"
#include "DX11GraphicsDevice.h"
#include "DX11VertexShader.h"
#include "DX11FragmentShader.h"

#include "d3dUtils.h"
#include "dxTrace.h"
#include <Core/Utils/Misc.h>

namespace crystal
{
	DX11ShaderProgram::DX11ShaderProgram(DX11GraphicsDevice* graphicsDevice,
		std::shared_ptr<VertexShader> vertexShader,
		std::shared_ptr<FragmentShader> fragmentShader,
		const UniformVariableCollection& uniforms)
		: m_pGraphicsDevice(graphicsDevice), m_vertexShader(vertexShader), m_fragmentShader(fragmentShader)
	{
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
		d3dUtils::D3D11SetDebugObjectName(m_pConstantBuffer.Get(), "Shader Constant Buffer");
		m_pConstantBufferData = std::make_unique<char[]>(size);
		m_constBufferSize = size;
	}

	DX11ShaderProgram::~DX11ShaderProgram()
	{}

	void DX11ShaderProgram::SetShaderResources(int startSlot, int count, ID3D11ShaderResourceView* const* srvBuffer, ID3D11SamplerState* const* samplerBuffer)
	{
		auto context = m_pGraphicsDevice->GetD3DDeviceContext();
		if (m_vertexShader != nullptr)
		{
			context->VSSetSamplers(startSlot, count, samplerBuffer);
			context->VSSetShaderResources(startSlot, count, srvBuffer);
		}
		if (m_fragmentShader != nullptr)
		{
			context->PSSetSamplers(startSlot, count, samplerBuffer);
			context->PSSetShaderResources(startSlot, count, srvBuffer);
		}
	}

	void DX11ShaderProgram::Apply()
	{
		auto context = m_pGraphicsDevice->GetD3DDeviceContext();

		// Map const buffer data to GPU
		if (m_constBufferDirty)
		{
			D3D11_MAPPED_SUBRESOURCE mappedData;
			HR(context->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
			memcpy_s(mappedData.pData, m_constBufferSize, &m_pConstantBufferData[0], m_constBufferSize);
			context->Unmap(m_pConstantBuffer.Get(), 0);
			m_constBufferDirty = false;
		}
		if (m_vertexShader != nullptr)
		{
			m_vertexShader->Bind();
			context->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
		}
		if (m_fragmentShader != nullptr)
		{
			m_fragmentShader->Bind();
			context->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
		}
	}

	void DX11ShaderProgram::SetUniform1f(const std::string& name, float value)
	{
		auto iter = m_uniformMap.find(name);
		if (iter == m_uniformMap.end())
		{
			throw std::exception("Cannot find uniform variable");
		}
		memcpy_s(&m_pConstantBufferData[iter->second], sizeof(float), &value, sizeof(float));
		m_constBufferDirty = true;
	}

	void DX11ShaderProgram::SetUniformMat4f(const std::string& name, const Matrix4f& value)
	{
		auto iter = m_uniformMap.find(name);
		if (iter == m_uniformMap.end())
		{
			throw std::exception("Cannot find uniform variable");
		}
		memcpy_s(&m_pConstantBufferData[iter->second], sizeof(Matrix4f), 
			crystal_value_ptr(glm::transpose(value)), sizeof(Matrix4f));
		m_constBufferDirty = true;
	}
}
