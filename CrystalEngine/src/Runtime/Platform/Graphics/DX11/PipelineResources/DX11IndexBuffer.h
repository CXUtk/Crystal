#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11IndexBuffer : public IIndexBuffer
	{
	public:
		DX11IndexBuffer(DX11GraphicsDevice* graphicsDevice,
			ComPtr<ID3D11Buffer> indexBuffer, DXGI_FORMAT indexFormat);
		~DX11IndexBuffer() override;

		void SetToCurrentContext(ID3D11DeviceContext* context, size_t offset = 0);
	private:
		DX11GraphicsDevice*			m_pGraphicsDevice{};
		ComPtr<ID3D11Buffer>		m_pBuffer{};
		DXGI_FORMAT					m_indexFormat;
	};
}