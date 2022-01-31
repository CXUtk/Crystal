#include "DX11IndexBuffer.h"
#include "DX11GraphicsDevice.h"
#include "d3dUtils.h"
#include "dxTrace.h"
#include <Core/Utils/Misc.h>


namespace crystal
{
	DX11IndexBuffer::DX11IndexBuffer(DX11GraphicsDevice* graphicsDevice,
			ComPtr<ID3D11Buffer> indexBuffer, DXGI_FORMAT indexFormat)
		: m_pGraphicsDevice(graphicsDevice), m_pBuffer(indexBuffer), m_indexFormat(indexFormat)
	{

	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{}

	void DX11IndexBuffer::Bind(size_t offset)
	{
		auto deviceContext = m_pGraphicsDevice->GetD3DDeviceContext();
		deviceContext->IASetIndexBuffer(m_pBuffer.Get(), m_indexFormat, offset);
	}
}
