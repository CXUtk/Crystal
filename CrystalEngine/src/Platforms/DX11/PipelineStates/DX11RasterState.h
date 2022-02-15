#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11RasterState : public IRasterState
	{
	public:
		DX11RasterState(DX11GraphicsDevice* graphicsDevice, const RasterStateDescription& rasterDesc);
		~DX11RasterState();

		void Load(DX11GraphicsContext* context);
		void Unload(DX11GraphicsContext* context);
	private:
		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11RasterizerState>	m_pRasterState = nullptr;
		std::unique_ptr<D3D11_VIEWPORT> m_pViewport = nullptr;
		bool							m_enableScissor = false;
		Bound2i							m_scissorBound{};
	};
}