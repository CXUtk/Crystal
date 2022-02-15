#pragma once
#include "../DX11Common.h"
#include <map>

namespace crystal
{
	class DX11ShaderProgram : public IShaderProgram
	{
	public:
		DX11ShaderProgram(DX11GraphicsDevice* graphicsDevice, std::shared_ptr<IVertexShader> vertexShader,
			std::shared_ptr<IFragmentShader> fragmentShader, const UniformVariableCollection& uniforms);
		~DX11ShaderProgram();

		//void SetShaderResources(int startSlot, int count, ID3D11ShaderResourceView* const* srvBuffer, ID3D11SamplerState* const* samplerBuffer);

		virtual void SetUniform1f(const std::string& name, float value) override;
		virtual void SetUniformMat4f(const std::string& name, const Matrix4f& value) override;
		virtual ShaderMask GetShaderMask() const override { return m_shaderMask; }

		void SetToCurrentContext(ID3D11DeviceContext* context);
	private:
		DX11GraphicsDevice*						m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11Buffer>					m_pConstantBuffer = nullptr;
		std::unique_ptr<char[]>					m_pConstantBufferData{};
		std::map<std::string, size_t>			m_uniformMap{};

		std::shared_ptr<DX11VertexShader>		m_vertexShader = nullptr;
		std::shared_ptr<DX11FragmentShader>		m_fragmentShader = nullptr;

		bool		m_constBufferDirty = true;
		size_t		m_constBufferSize = 0;
		ShaderMask	m_shaderMask = ShaderMask::CRYSTAL_SHADERMASK_NO_SHADER;
	};
}