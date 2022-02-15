#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11BlendState : public IBlendState
	{
	public:
		DX11BlendState(DX11GraphicsDevice* graphicsDevice, 
			const BlendStateDescription& blendStateDesc);
		~DX11BlendState();

		void Load(DX11GraphicsContext* context);
		void Unload(DX11GraphicsContext* context);
	private:
		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11BlendState>		m_pBlendState = nullptr;
	};
}