#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11DepthStencilState : public IDepthStencilState
	{
	public:
		DX11DepthStencilState(DX11GraphicsDevice* graphicsDevice, const DepthStencilStateDescription& DSDesc);
		~DX11DepthStencilState();

		void Load(DX11GraphicsContext* context);
		void Unload(DX11GraphicsContext* context);
	private:
		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState = nullptr;
	};
}