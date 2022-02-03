#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11VertexBuffer : public IVertexBuffer
	{
	public:
		DX11VertexBuffer(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11Buffer> buffer);
		~DX11VertexBuffer() override;

		void BindVertexLayout(const VertexLayout& layout) override;
		virtual void Bind(size_t offset) override;

	private:
		DX11GraphicsDevice*			m_pGraphicsDevice{};
		ComPtr<ID3D11Buffer>		m_pBuffer{};
		ComPtr<ID3D11InputLayout>	m_pInputLayout{};
		UINT						m_strides = 0;
	};

}