#pragma once
#include <Platforms/DX11/DX11Common.h>

namespace crystal
{
	class DX11BlendState
	{
	public:
		DX11BlendState(DX11GraphicsDevice* graphicsDevice, 
			ComPtr<ID3D11BlendState> blendState);
		~DX11BlendState();

		void Apply();
	private:
		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11BlendState>		m_blendState = nullptr;
	};
}