#include "DX11GraphicsDevice.h"
#include "d3dUtils.h"
#include "dxTrace.h"

#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "DX11VertexShader.h"
#include "DX11FragmentShader.h"
#include "DX11ShaderProgram.h"
#include "DX11PipelineStateObject.h"
#include "DX11Texture2D.h"
#include "WICTextureLoader.h"

#include <Core/InitArgs.h>
#include <Core/Utils/Misc.h>
#include <Core/Utils/IO.h>
#include <SJson/SJson.h>
#include <glm/gtx/quaternion.hpp>

namespace crystal
{
	DX11GraphicsDevice::DX11GraphicsDevice(const InitArgs& args, Win32GameWindow* window)
		: m_pWindow(window), m_Enable4xMsaa(args.Enable4xMSAA)
	{
		if (!m_initD3DX11())
		{
			throw std::exception("[DX11GraphicsDevice::DX11GraphicsDevice] Unable to start Dx11");
		}
	}

	DX11GraphicsDevice::~DX11GraphicsDevice()
	{}

	void DX11GraphicsDevice::SetPipelineStateObject(std::shared_ptr<PipelineStateObject> pso)
	{
		pso->Apply();
	}

	void DX11GraphicsDevice::Clear(ClearOptions options, const Color4f & color, float depth, int stencil)
	{
		if (options & ClearOptions::Target)
		{
			m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(),
				crystal_value_ptr(color));
		}

		int clearFlag = 0;
		if (options & ClearOptions::Depth)
		{
			clearFlag |= D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH;
		}
		if (options & ClearOptions::Stencil)
		{
			clearFlag |= D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL;
		}
		m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), clearFlag, depth, stencil);
	}

	void DX11GraphicsDevice::Present()
	{
		HR(m_pSwapChain->Present(0, 0));
	}

	std::shared_ptr<PipelineStateObject> DX11GraphicsDevice::CreatePipelineStateObject()
	{
		return std::make_shared<DX11PipelineStateObject>(this);
	}

	std::shared_ptr<VertexBuffer> DX11GraphicsDevice:: CreateVertexBuffer(const VertexBufferDescription& desc, void* src, size_t size)
	{
		ComPtr<ID3D11Buffer> vertexBuffer = CreateBuffer(src, size, desc.Usage, D3D11_BIND_VERTEX_BUFFER);
		return std::make_shared<DX11VertexBuffer>(this, vertexBuffer);
	}

	std::shared_ptr<IndexBuffer> DX11GraphicsDevice::CreateIndexBuffer(const IndexBufferDescription& desc, 
		void* src, size_t size)
	{
		ComPtr<ID3D11Buffer> indexBuffer = CreateBuffer(src, size, desc.Usage, D3D11_BIND_INDEX_BUFFER);
		return std::make_shared<DX11IndexBuffer>(this, indexBuffer, DX11Common::DataFormatConvert(desc.Format));
	}

	std::shared_ptr<VertexShader> DX11GraphicsDevice::CreateVertexShaderFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint)
	{
		auto pBlob = m_getShaderBlobFromMemory(src, size, name, entryPoint, ShaderType::VERTEX_SHADER);
		ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
		HR(m_pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
			nullptr, pVertexShader.GetAddressOf()));
		return std::make_shared<DX11VertexShader>(this, pVertexShader);
	}

	std::shared_ptr<FragmentShader> DX11GraphicsDevice::CreateFragmentShaderFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint)
	{
		auto pBlob = m_getShaderBlobFromMemory(src, size, name, entryPoint, ShaderType::FRAGMENT_SHADER);
		ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
		HR(m_pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
			nullptr, pPixelShader.GetAddressOf()));
		return std::make_shared<DX11FragmentShader>(this, pPixelShader);
	}


	UniformVariable ParseUniformVariable(const std::string& name, const std::string& type)
	{
		UniformVariable varb;
		varb.Name = name;
		varb.Format = StringToComponentFormatConvert(type);
		return varb;
	}

	std::shared_ptr<ShaderProgram> DX11GraphicsDevice::CreateShaderProgramFromFile(const std::string& path)
	{
		auto source = ReadAllStringFromFile(path);
		auto root = SJson::SJsonParse(source);
		auto dx11Src = root->GetMember("dx11Src");
		auto directory = GetDirectoryPath(path);

		auto filePath = directory + "/" + dx11Src->GetString();
		auto shaderSrc = ReadAllStringFromFile(filePath);

		UniformVariableCollection variables;
		auto uniforms = root->GetMember("uniforms");
		uniforms->ForEachElements([&variables](const SJson::SJsonNode* node)
		{
			UniformVariable variable = ParseUniformVariable(node->GetMember("name")->GetString(),
				node->GetMember("type")->GetString());
			variables.Add(variable);
		});
		auto vs = this->CreateVertexShaderFromMemory(shaderSrc.c_str(), shaderSrc.size(), 
			dx11Src->GetString(), root->GetMember("vsEntry")->GetString());
		auto fs = this->CreateFragmentShaderFromMemory(shaderSrc.c_str(), shaderSrc.size(),
			dx11Src->GetString(), root->GetMember("fsEntry")->GetString());

		return std::make_shared<DX11ShaderProgram>(this, vs, fs, variables);
	}

	std::shared_ptr<Texture2D> DX11GraphicsDevice::CreateTexture2D(const std::string& path, const Texture2DDescription& texDesc)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.MipLevels = texDesc.MipmapLevels;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DX11Common::RenderFormatConvert(texDesc.Format, true);
		textureDesc.Usage = DX11Common::BufferUsageToDX11Convert(texDesc.Usage);
		textureDesc.MiscFlags = 0;
		textureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		if (texDesc.Usage == BufferUsage::CPURead)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
		}
		else if (texDesc.Usage == BufferUsage::CPUWrite)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
		else if (texDesc.Usage == BufferUsage::CPURWrite)
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
		ComPtr<ID3D11ShaderResourceView> textureSRV;
		DirectX::CreateWICTextureFromFileEx(m_pd3dDevice.Get(), DX11Common::ConvertFromUtf8ToUtf16(path).c_str(),
			0, textureDesc.Usage, textureDesc.BindFlags, textureDesc.CPUAccessFlags,
			textureDesc.MiscFlags, 0, nullptr, textureSRV.GetAddressOf());

		return std::make_shared<Texture2D>(this, nullptr, textureSRV);
	}

	void DX11GraphicsDevice::DrawPrimitives(PrimitiveType primitiveType, size_t offset, size_t numVertices)
	{
		m_pd3dImmediateContext->IASetPrimitiveTopology(DX11Common::PrimitiveTypeToTopologyConvert(primitiveType));
		m_pd3dImmediateContext->Draw(numVertices, offset);
	}

	void DX11GraphicsDevice::DrawIndexedPrimitives(PrimitiveType primitiveType, size_t numIndices, size_t indexOffset, size_t vertexOffset)
	{
		m_pd3dImmediateContext->IASetPrimitiveTopology(DX11Common::PrimitiveTypeToTopologyConvert(primitiveType));
		m_pd3dImmediateContext->DrawIndexed(numIndices, indexOffset, vertexOffset);
	}


	bool DX11GraphicsDevice::m_initD3DX11()
	{
		HRESULT hr = S_OK;

		// Create D3D device and D3D device context
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
		D3D_FEATURE_LEVEL featureLevels[1] = { D3D_FEATURE_LEVEL_11_0 };
		UINT numFeatureLevels = 1;

		D3D_FEATURE_LEVEL curFeatureLevel;
		D3D_DRIVER_TYPE d3dDriverType;

		d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(),
			&curFeatureLevel, m_pd3dImmediateContext.GetAddressOf());

		if (FAILED(hr))
		{
			MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
			return false;
		}

		// 检测是否支持特性等级11.0
		if (curFeatureLevel != D3D_FEATURE_LEVEL_11_0 && curFeatureLevel != D3D_FEATURE_LEVEL_11_1)
		{
			MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		// 检测 MSAA支持的质量等级
		m_pd3dDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
		assert(m_4xMsaaQuality > 0);


		ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
		ComPtr<IDXGIFactory> dxgiFactory = 0;
		HR(m_pd3dDevice.As(&dxgiDevice));
		HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
		HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())));

		auto clientSize = m_pWindow->GetWindowSize();
		m_oldClientSize = clientSize;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = clientSize.x;
		sd.BufferDesc.Height = clientSize.y;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// 是否开启4倍多重采样？
		if (m_Enable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_pWindow->GetHWND();
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		HR(dxgiFactory->CreateSwapChain(m_pd3dDevice.Get(), &sd, m_pSwapChain.GetAddressOf()));

		dxgiFactory->MakeWindowAssociation(m_pWindow->GetHWND(), DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

		// 设置调试对象名
		d3dUtils::D3D11SetDebugObjectName(m_pd3dImmediateContext.Get(), "ImmediateContext");
		d3dUtils::DXGISetDebugObjectName(m_pSwapChain.Get(), "SwapChain");

		m_resizeBuffer();
		m_pWindow->AppendOnResizeEvent([this](Vector2i size) {
			m_resizeBuffer();
		});
		return true;
	}

	void DX11GraphicsDevice::m_resizeBuffer()
	{
		assert(m_pd3dImmediateContext);
		assert(m_pd3dDevice);
		assert(m_pSwapChain);

		// Release previous buffer resources
		m_pRenderTargetView.Reset();
		m_pDepthStencilView.Reset();
		m_pDepthStencilBuffer.Reset();

		// Resize the buffer if window size was changed
		auto newWindowSize = m_pWindow->GetWindowSize();
		if (m_oldClientSize != newWindowSize)
		{
			HR(m_pSwapChain->ResizeBuffers(1, newWindowSize.x, newWindowSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
			m_oldClientSize = newWindowSize;
		}

		// Create the render target view
		{
			ComPtr<ID3D11Texture2D> backBuffer;

			m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
			m_pd3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());

			// Set DEBUG name
			d3dUtils::D3D11SetDebugObjectName(backBuffer.Get(), "BackBuffer[0]");
		}

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = newWindowSize.x;
		depthStencilDesc.Height = newWindowSize.y;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// Check MSAA state
		if (m_Enable4xMsaa)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}

		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		// Create DepthStencil buffer and view
		HR(m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf()));
		HR(m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf()));
	
		// Combine render targets and the depth stencil buffer to the pipeline
		m_pd3dImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

		// Initialize viewport
		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = static_cast<float>(newWindowSize.x);
		m_ScreenViewport.Height = static_cast<float>(newWindowSize.y);
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;

		m_pd3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);

		// Set DEBUG name
		d3dUtils::D3D11SetDebugObjectName(m_pDepthStencilBuffer.Get(), "DepthStencilBuffer");
		d3dUtils::D3D11SetDebugObjectName(m_pDepthStencilView.Get(), "DepthStencilView");
		d3dUtils::D3D11SetDebugObjectName(m_pRenderTargetView.Get(), "BackBufferRTV[0]");
	}

	ComPtr<ID3DBlob> DX11GraphicsDevice::m_getShaderBlobFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint, ShaderType type)
	{
		ComPtr<ID3DBlob> pBlob = nullptr;
		HR(d3dUtils::CreateShaderFromMemory(src, size, name.c_str(), entryPoint.c_str(), DX11Common::ShaderModelConvert(type), pBlob.GetAddressOf()));
		return pBlob;
	}

	ComPtr<ID3D11Buffer> DX11GraphicsDevice::CreateBuffer(void* src, size_t size, BufferUsage usage, UINT bindFlags)
	{
		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(vbd));
		vbd.Usage = DX11Common::BufferUsageToDX11Convert(usage);
		vbd.ByteWidth = size;
		vbd.BindFlags = bindFlags;
		vbd.CPUAccessFlags = 0;

		if (usage == BufferUsage::CPURead)
		{
			vbd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
		}
		else if (usage == BufferUsage::CPUWrite)
		{
			vbd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
		else if (usage == BufferUsage::CPURWrite)
		{
			vbd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = src;

		ComPtr<ID3D11Buffer> pBuffer;
		HR(m_pd3dDevice->CreateBuffer(&vbd, src ? &initData : nullptr, pBuffer.GetAddressOf()));
		return pBuffer;
	}
}
