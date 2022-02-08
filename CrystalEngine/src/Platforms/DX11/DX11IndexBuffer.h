#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11IndexBuffer : public IIndexBuffer
	{
	public:
		DX11IndexBuffer(DX11GraphicsDevice* graphicsDevice, 
			ComPtr<ID3D11Buffer> indexBuffer, DXGI_FORMAT indexFormat);
		~DX11IndexBuffer() override;

		void Bind(size_t offset);
	private:
		DX11GraphicsDevice*			m_pGraphicsDevice{};
		ComPtr<ID3D11Buffer>		m_pBuffer{};
		DXGI_FORMAT					m_indexFormat;
	};

}