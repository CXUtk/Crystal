#include "DX11IndexBuffer.h"
#include "../DX11GraphicsDevice.h"
#include "../d3dUtils.h"
#include "../dxTrace.h"
#include <Core/Utils/Misc.h>


namespace crystal
{
	DX11IndexBuffer::DX11IndexBuffer(DX11GraphicsDevice* graphicsDevice,
			ComPtr<ID3D11Buffer> indexBuffer, DXGI_FORMAT indexFormat)
		: m_pGraphicsDevice(graphicsDevice), m_pBuffer(indexBuffer), m_indexFormat(indexFormat)
	{
		d3dUtils::D3D11SetDebugObjectName(m_pBuffer.Get(), "IndexBuffer");
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{}

	void DX11IndexBuffer::SetToCurrentContext(ID3D11DeviceContext * context, size_t offset)
	{
		context->IASetIndexBuffer(m_pBuffer.Get(), m_indexFormat, offset);
	}
}
