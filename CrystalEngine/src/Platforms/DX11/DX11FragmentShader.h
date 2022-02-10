#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11FragmentShader : public IFragmentShader
	{
	public:
		DX11FragmentShader(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11PixelShader> pixelShader);
		~DX11FragmentShader() override;

	private:
		DX11GraphicsDevice*			m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11PixelShader>	m_pPixelShader = nullptr;

		friend DX11ShaderProgram;
		void m_BindToPipeline();
	};
}