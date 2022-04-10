#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11PipelineStateObject : public IPipelineStateObject
	{
	public:
		DX11PipelineStateObject(DX11GraphicsDevice* graphicsDevice, DX11GraphicsContext* graphicsContext);
		~DX11PipelineStateObject() override;

		virtual void SetRasterState(std::shared_ptr<IRasterState> rasterState) override;
		virtual void SetDepthStencilState(std::shared_ptr<IDepthStencilState> depthStencilState) override;
		virtual void SetBlendState(std::shared_ptr<IBlendState> blendState) override;

        virtual std::shared_ptr<IRasterState> GetRasterState() override;
        virtual std::shared_ptr<IDepthStencilState> GetDepthStencilState() override;
        virtual std::shared_ptr<IBlendState> GetBlendState() override;

		void Load();
		void Unload();
	private:

		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		DX11GraphicsContext*			m_pGraphicsContext = nullptr;

		std::shared_ptr<DX11RasterState>		m_pRasterizerState = nullptr;
		std::shared_ptr<DX11BlendState>			m_pBlendState = nullptr;
		std::shared_ptr<DX11DepthStencilState>	m_pDepthStencilState = nullptr;
	};
}