#pragma once
#include "DX11Common.h"

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

	class DX11ShaderProgram : public IShaderProgram
	{
	public:
		DX11ShaderProgram(std::shared_ptr<IVertexShader> vertexShader,
			std::shared_ptr<IFragmentShader> fragmentShader);
		~DX11ShaderProgram();

		virtual void Apply() override;

	private:
		std::vector<std::shared_ptr<IShader>> m_shaders{};
	};
}