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

	class DX11ShaderProgram : public IShaderProgram
	{
	public:
		DX11ShaderProgram(DX11GraphicsDevice* graphicsDevice, std::shared_ptr<IVertexShader> vertexShader,
			std::shared_ptr<IFragmentShader> fragmentShader, const UniformVariableCollection& uniforms);
		~DX11ShaderProgram();

		virtual void Apply() override;
		virtual void SetUniform1f(const std::string& name, float value) override;

	private:
		DX11GraphicsDevice*						m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11Buffer>					m_pConstantBuffer = nullptr;
		std::vector<std::shared_ptr<IShader>>	m_shaders{};
		std::unique_ptr<char[]>					m_pConstantBufferData{};
		std::map<std::string, size_t>			m_uniformMap{};

		bool	m_constBufferDirty = true;
		size_t	m_constBufferSize = 0;
	};
}