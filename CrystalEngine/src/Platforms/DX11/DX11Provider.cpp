#include "DX11Provider.h"
#include <Platforms/Windows32/Win32GameWindow.h>

namespace crystal
{
	DX11Provider::DX11Provider(const InitArgs& args)
	{
		_gameWindow = std::make_unique<Win32GameWindow>(args, GetModuleHandle(NULL));
	}

	DX11Provider::~DX11Provider()
	{}

	IGameWindow* DX11Provider::GetGameWindow() const
	{
		return ptr(_gameWindow);
	}

	IFileSystem* DX11Provider::GetFileSystem() const
	{
		return nullptr;
	}

	bool DX11Provider::initDX11()
	{
		HRESULT hr = S_OK;

		// 创建D3D设备 和 D3D设备上下文
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		// 驱动类型数组
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		// 特性等级数组
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		D3D_FEATURE_LEVEL featureLevel;
		D3D_DRIVER_TYPE d3dDriverType;
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			d3dDriverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(), &featureLevel, m_pd3dImmediateContext.GetAddressOf());

			if (hr == E_INVALIDARG)
			{
				// Direct3D 11.0 的API不承认D3D_FEATURE_LEVEL_11_1，所以我们需要尝试特性等级11.0以及以下的版本
				hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
					D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(), &featureLevel, m_pd3dImmediateContext.GetAddressOf());
			}

			if (SUCCEEDED(hr))
				break;
		}

		if (FAILED(hr))
		{
			MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
			return false;
		}

		// 检测是否支持特性等级11.0或11.1
		if (featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
		{
			MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		// 检测 MSAA支持的质量等级
		m_pd3dDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
		assert(m_4xMsaaQuality > 0);
	}
}
