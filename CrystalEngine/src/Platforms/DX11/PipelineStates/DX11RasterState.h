#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11RasterState : public IRasterState
	{
	public:
		DX11RasterState(DX11GraphicsDevice* graphicsDevice, const RasterStateDescription& rasterDesc);
		~DX11RasterState();

        virtual void SetScissorState(bool enable) override;
        virtual bool GetScissorState() const override;

        virtual void SetScissorBound(const Bound2i& scissorBound) override;
        virtual Bound2i GetScissorBound() const override;

		void Load(DX11GraphicsContext* context);
		void Unload(DX11GraphicsContext* context);
	private:
		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;

        D3D11_RASTERIZER_DESC           m_RSDesc{};
		ComPtr<ID3D11RasterizerState>	m_pRasterState = nullptr;
		bool							m_shouldReGenerateRS = true;

		Bound2i							m_scissorBound{};
	};
}