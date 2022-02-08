#include "DX11SamplerState.h"
#include "DX11Texture2D.h"
#include "DX11ShaderProgram.h"
#include "DX11GraphicsDevice.h"
#include "DX11VertexShader.h"
#include "DX11FragmentShader.h"
#include "dxTrace.h"
#include "d3dUtils.h"

namespace crystal
{
	static std::shared_ptr<SamplerState> PointClamp = nullptr;
	static std::shared_ptr<SamplerState> PointWarp = nullptr;
	static std::shared_ptr<SamplerState> LinearClamp = nullptr;
	static std::shared_ptr<SamplerState> LinearWarp = nullptr;

	static void initStatics()
	{
		
	}

	DX11SamplerState::DX11SamplerState(DX11GraphicsDevice* graphicsDevice, ComPtr<ID3D11SamplerState> samplerState)
		: m_pGraphicsDevice(graphicsDevice), m_samplerState(samplerState)
	{

	}

	DX11SamplerState::~DX11SamplerState()
	{}


	void DX11SamplerState::Init(DX11GraphicsDevice* graphicsDevice)
	{
		auto d3dDevice = graphicsDevice->GetD3DDevice();
		ComPtr<ID3D11SamplerState> pointClamp;
		ComPtr<ID3D11SamplerState> pointWarp;
		ComPtr<ID3D11SamplerState> linearClamp;		
		ComPtr<ID3D11SamplerState> linearWarp;
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HR(d3dDevice->CreateSamplerState(&sampDesc, pointClamp.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(pointClamp.Get(), "Point Clamp Sampler");
		PointClamp = std::make_shared<SamplerState>(graphicsDevice, pointClamp);

		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		HR(d3dDevice->CreateSamplerState(&sampDesc, pointWarp.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(pointWarp.Get(), "Point Warp Sampler");
		PointWarp = std::make_shared<SamplerState>(graphicsDevice, pointWarp);

		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		HR(d3dDevice->CreateSamplerState(&sampDesc, linearWarp.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(linearWarp.Get(), "Linear Warp Sampler");
		LinearWarp = std::make_shared<SamplerState>(graphicsDevice, linearWarp);

		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		HR(d3dDevice->CreateSamplerState(&sampDesc, linearClamp.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(linearClamp.Get(), "Linear Clamp Sampler");
		LinearClamp = std::make_shared<SamplerState>(graphicsDevice, linearClamp);
	}

	std::shared_ptr<SamplerState> DX11SamplerState::GetSamplerState(SamplerStates state)
	{
		switch (state)
		{
		case crystal::SamplerStates::PointClamp:
		{
			return PointClamp;
		}
		break;
		case crystal::SamplerStates::PointWarp:
		{
			return PointWarp;
		}
			break;
		case crystal::SamplerStates::LinearClamp:
		{
			return LinearClamp;
		}
			break;
		case crystal::SamplerStates::LinearWarp:
		{
			return LinearWarp;
		}
			break;
		default:
			break;
		}
		throw std::exception("Unknown Sampler State");
	}
}
