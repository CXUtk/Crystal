﻿#pragma once
#include "DX11Common.h"

namespace crystal
{
	class DX11SamplerState
	{
	public:
		DX11SamplerState(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11SamplerState> samplerState);
		~DX11SamplerState();

		ComPtr<ID3D11SamplerState> GetDX11SamplerState() const { return m_samplerState.Get(); }
	private:
		DX11GraphicsDevice*				m_pGraphicsDevice = nullptr;
		ComPtr<ID3D11SamplerState>		m_samplerState = nullptr;
	};
}