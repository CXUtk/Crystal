#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11FragmentShader : public IFragmentShader
	{
	public:
		DX11FragmentShader(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11PixelShader> pixelShader);
		~DX11FragmentShader() override;

		void Bind();
	private:
		DX11GraphicsDevice*			m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11PixelShader>	m_pPixelShader = nullptr;
	};
}