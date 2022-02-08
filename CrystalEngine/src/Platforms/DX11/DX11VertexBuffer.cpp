#include "DX11VertexBuffer.h"
#include "DX11GraphicsDevice.h"
#include "d3dUtils.h"
#include "dxTrace.h"
#include <Core/Utils/Misc.h>


namespace crystal
{
	DX11VertexBuffer::DX11VertexBuffer(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11Buffer> buffer)
		: m_pGraphicsDevice(graphicsDevice), m_pBuffer(buffer)
	{
		d3dUtils::D3D11SetDebugObjectName(m_pBuffer.Get(), "Vertex Buffer");
	}


	std::string CreateDummyVertexShader(const VertexLayout& layout)
	{
		std::string components;
		int index = 0;
		for (auto& element : layout.Elements)
		{
			index++;
			auto name = DX11Common::RenderFormatToShaderVarConvert(element.Format);
			auto semantic = DX11Common::SemanticNameConvert(element.Semantic);
			components.append(string_format("%s %s:%s%d; ", name, GetNameByIndex(index).c_str(), semantic, element.SemanticIndex));
		}
		return string_format("struct VIn { %s };\
struct VOut { float4 Pos:SV_POSITION; };\
VOut VS(VIn vin)\
{\
VOut vout;\
vout.Pos=float4(0,0,0,0);\
return vout;\
}", components.c_str());
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{}

	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> ConvertToD3D11InputElements(const VertexLayout& layout)
	{
		int count = layout.Elements.size();
		auto result = std::make_unique<D3D11_INPUT_ELEMENT_DESC[]>(count);
		for (int i = 0; i < count; i++)
		{
			ZeroMemory(&result[i], sizeof(D3D11_INPUT_ELEMENT_DESC));
			auto& elementDesc = result[i];
			auto& layoutElement = layout.Elements[i];

			elementDesc.AlignedByteOffset = layoutElement.ByteOffset;
			elementDesc.Format = DX11Common::RenderFormatConvert(layoutElement.Format, false);
			elementDesc.SemanticName = DX11Common::SemanticNameConvert(layoutElement.Semantic);
			elementDesc.SemanticIndex = layoutElement.SemanticIndex;
		}
		return result;
	}

	void DX11VertexBuffer::BindVertexLayout(const VertexLayout& layout)
	{
		auto device = m_pGraphicsDevice->GetD3DDevice();
		auto d3dInputElements = ConvertToD3D11InputElements(layout);
		m_strides = layout.Stride;

		// Hack: create a dummy vertex shader to genereate InputLayout
		auto dummyShader = CreateDummyVertexShader(layout);

		ComPtr<ID3DBlob> pBlob = nullptr;
		HR(d3dUtils::CreateShaderFromMemory(dummyShader.c_str(), dummyShader.size(), "dummyVS", "VS", 
			DX11Common::ShaderModelConvert(ShaderType::VERTEX_SHADER), pBlob.GetAddressOf()));
		HR(device->CreateInputLayout(d3dInputElements.get(), layout.Elements.size(),
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), m_pInputLayout.GetAddressOf()));
	}

	void DX11VertexBuffer::Bind(size_t offset)
	{
		auto deviceContext = m_pGraphicsDevice->GetD3DDeviceContext();
		UINT offsetU = offset;
		deviceContext->IASetVertexBuffers(0, 1, m_pBuffer.GetAddressOf(), &m_strides, &offsetU);
		deviceContext->IASetInputLayout(m_pInputLayout.Get());
	}
}
