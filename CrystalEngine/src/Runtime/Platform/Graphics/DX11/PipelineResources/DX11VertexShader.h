#pragma once
#include "../DX11Common.h"

namespace crystal
{
	class DX11VertexShader : public IVertexShader
	{
	public:
		DX11VertexShader(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11VertexShader> vertexShader);
		~DX11VertexShader() override;

		void SetToCurrentContext(ID3D11DeviceContext* context);
	private:
		DX11GraphicsDevice*			m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11VertexShader>	m_pVertexShader = nullptr;
	};
}