#include "DX11RasterState.h"

#include "../DX11GraphicsContext.h"
#include "../DX11GraphicsDevice.h"
#include "../dxTrace.h"

namespace crystal
{

	DX11RasterState::DX11RasterState(DX11GraphicsDevice* graphicsDevice,
		const RasterStateDescription& rasterDesc)
		: m_pGraphicsDevice(graphicsDevice)
	{
        m_RSDesc.FillMode = DX11Common::FillModeConvert(rasterDesc.FillMode);
        m_RSDesc.CullMode = DX11Common::CullModeConvert(rasterDesc.CullMode);
        m_RSDesc.FrontCounterClockwise = false;
        m_RSDesc.DepthBias = 0;
        m_RSDesc.DepthBiasClamp = 0.0f;
        m_RSDesc.SlopeScaledDepthBias = 0.0f;
        m_RSDesc.DepthClipEnable = true;
        m_RSDesc.ScissorEnable = rasterDesc.EnableScissorTest;
        m_RSDesc.MultisampleEnable = false;
        m_RSDesc.AntialiasedLineEnable = rasterDesc.EnableAntialiasedLine;

        if (rasterDesc.EnableScissorTest)
        {
            m_scissorBound = rasterDesc.ScissorBound;
        }
        else
        {
            m_scissorBound = Bound2i(Vector2i(std::numeric_limits<int>::min()),
                Vector2i(std::numeric_limits<int>::max()));
        }
	}

	DX11RasterState::~DX11RasterState()
	{}

    void DX11RasterState::SetScissorState(bool enable)
    {
        if (enable != m_RSDesc.ScissorEnable)
        {
            m_RSDesc.ScissorEnable = enable;
            m_shouldReGenerateRS = true;
        }
    }

    bool DX11RasterState::GetScissorState() const
    {
        return m_RSDesc.ScissorEnable;
    }

    void DX11RasterState::SetScissorBound(const Bound2i& scissorBound)
    {
        m_scissorBound = scissorBound;
    }

    Bound2i DX11RasterState::GetScissorBound() const
    {
        return m_scissorBound;
    }

	void DX11RasterState::Load(DX11GraphicsContext* context)
	{
		auto dx11Context = context->GetD3DContext();

        if (m_shouldReGenerateRS)
        {
            HR(m_pGraphicsDevice->GetD3DDevice()->CreateRasterizerState(&m_RSDesc,
                m_pRasterState.ReleaseAndGetAddressOf()));
            m_shouldReGenerateRS = false;
        }

        dx11Context->RSSetState(m_pRasterState.Get());

        if (m_RSDesc.ScissorEnable)
        {
            auto minpos = m_scissorBound.GetMinPos();
            auto maxpos = m_scissorBound.GetMaxPos();
            auto bufferSize = context->GetBackBufferSize();

            RECT scissorRect = {};
            scissorRect.left = minpos.x;
            scissorRect.top = bufferSize.y - maxpos.y;
            scissorRect.right = maxpos.x;
            scissorRect.bottom = bufferSize.y - minpos.y;
            dx11Context->RSSetScissorRects(1, &scissorRect);
        }
	}

	void DX11RasterState::Unload(DX11GraphicsContext* context)
	{
	}
}