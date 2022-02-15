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
		virtual void SetDepthStencilState(std::shared_ptr<IDepthStencilState> rasterState) override;
		virtual void SetBlendState(std::shared_ptr<IBlendState> rasterState) override;
		virtual void SetViewPort(const Bound3f* viewPortBound) override;

		void Load();
		void Unload();
	private:

		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		DX11GraphicsContext*			m_pGraphicsContext = nullptr;

		std::shared_ptr<IRasterState>		m_currentRasterizerState = nullptr;
		std::shared_ptr<IBlendState>		m_currentBlendState = nullptr;
		std::shared_ptr<IDepthStencilState>	m_currentDepthStencilState = nullptr;
	};
}