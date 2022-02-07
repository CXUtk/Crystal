#pragma once
#include "DX11Common.h"
#include <map>

namespace crystal
{
	class DX11ShaderProgram : public IShaderProgram
	{
	public:
		DX11ShaderProgram(DX11GraphicsDevice* graphicsDevice, std::shared_ptr<VertexShader> vertexShader,
			std::shared_ptr<FragmentShader> fragmentShader, const UniformVariableCollection& uniforms);
		~DX11ShaderProgram();

		void SetShaderResources(int startSlot, int count, ID3D11ShaderResourceView* const* srvBuffer, ID3D11SamplerState* const* samplerBuffer);
		virtual void Apply() override;
		virtual void SetUniform1f(const std::string& name, float value) override;
		virtual void SetUniformMat4f(const std::string& name, const Matrix4f& value) override;

	private:
		DX11GraphicsDevice* m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11Buffer>					m_pConstantBuffer = nullptr;
		std::unique_ptr<char[]>					m_pConstantBufferData{};
		std::map<std::string, size_t>			m_uniformMap{};

		std::shared_ptr<VertexShader>			m_vertexShader = nullptr;
		std::shared_ptr<FragmentShader>			m_fragmentShader = nullptr;
		//std::shared_ptr<VertexShader>			m_vertexShader = nullptr;
		//std::shared_ptr<VertexShader>			m_vertexShader = nullptr;

		bool	m_constBufferDirty = true;
		size_t	m_constBufferSize = 0;
	};
}