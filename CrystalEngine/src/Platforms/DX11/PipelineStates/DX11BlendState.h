#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11BlendState : public IBlendState
	{
	public:
		DX11BlendState(DX11GraphicsDevice* graphicsDevice, 
			ComPtr<ID3D11BlendState> blendState);
		~DX11BlendState();

		void SetToCurrentContext(ID3D11DeviceContext* context);
	private:
		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11BlendState>		m_blendState = nullptr;
	};
}