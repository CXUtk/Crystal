#pragma once
#include "DX11Common.h"
#include <map>

namespace crystal
{
	class DX11VertexShader : public IVertexShader
	{
	public:
		DX11VertexShader(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11VertexShader> vertexShader);
		~DX11VertexShader() override;

		virtual ShaderType GetShaderType() const override { return ShaderType::VERTEX_SHADER; }
		virtual void Apply() override;

	private:
		DX11GraphicsDevice*			m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11VertexShader>	m_pVertexShader = nullptr;
	};

	class DX11FragmentShader : public IFragmentShader
	{
	public:
		DX11FragmentShader(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11PixelShader> pixelShader);
		~DX11FragmentShader() override;

		virtual ShaderType GetShaderType() const override { return ShaderType::FRAGMENT_SHADER; }
		virtual void Apply() override;

	private:
		DX11GraphicsDevice*			m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11PixelShader>	m_pPixelShader = nullptr;
	};

	
}