#pragma once
#include "GraphicsCommon.h"

namespace crystal
{
	class IPipelineStateObject
	{
	public:
		virtual ~IPipelineStateObject() = 0 {};

		virtual void SetRasterState(std::shared_ptr<IRasterState> rasterState) = 0;
		virtual void SetDepthStencilState(std::shared_ptr<IDepthStencilState> DSState) = 0;
		virtual void SetBlendState(std::shared_ptr<IBlendState> blendStates) = 0;

        virtual std::shared_ptr<IRasterState> GetRasterState() = 0;
        virtual std::shared_ptr<IDepthStencilState> GetDepthStencilState() = 0;
        virtual std::shared_ptr<IBlendState> GetBlendState() = 0;
	};
}