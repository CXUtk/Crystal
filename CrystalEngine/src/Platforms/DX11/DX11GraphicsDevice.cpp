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
#include "DX11RenderTarget2D.h"
#include "DX11SamplerState.h"
#include "PipelineStates/DX11BlendState.h"

#include <Core/InitArgs.h>
#include <Core/Utils/Misc.h>
#include <Core/Utils/IO.h>
#include <SJson/SJson.h>
#include <glm/gtx/quaternion.hpp>

namespace crystal
{

	class DX11GraphicsDevice::CommonStates
	{
	public:
		CommonStates(DX11GraphicsDevice* graphicsDevice);

		std::shared_ptr<SamplerState> PointClamp = nullptr;
		std::shared_ptr<SamplerState> PointWarp = nullptr;
		std::shared_ptr<SamplerState> LinearClamp = nullptr;
		std::shared_ptr<SamplerState> LinearWarp = nullptr;


		std::shared_ptr<BlendState>		Blend_Opaque = nullptr;
		std::shared_ptr<BlendState>		Blend_Alpha = nullptr;
		std::shared_ptr<BlendState>		Blend_Additive = nullptr;

	private:
		DX11GraphicsDevice*		m_pGraphicsDevice;
	};

	DX11GraphicsDevice::DX11GraphicsDevice(const InitArgs& args, Win32GameWindow* window)
		: m_pWindow(window), m_Enable4xMsaa(args.Enable4xMSAA)
	{
		if (!m_initD3DX11())
		{
			throw std::exception("[DX11GraphicsDevice::DX11GraphicsDevice] Unable to start Dx11");
		}

		m_PSOStack[0] = CreatePipelineStateObject();
		
		m_commonStates = std::make_unique<CommonStates>(this);
	}

	DX11GraphicsDevice::~DX11GraphicsDevice()
	{}


	void DX11GraphicsDevice::Clear(ClearOptions options, const Color4f & color, float depth, int stencil)
	{
		if (options & ClearOptions::CRYSTAL_CLEAR_TARGET)
		{
			// On render target null
			if (m_renderTargetStackPtr == 0)
			{
				m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(),
					crystal_value_ptr(color));
			}
			else
			{
				m_pd3dImmediateContext->ClearRenderTargetView(m_renderTarget2DStack[m_renderTargetStackPtr]->GetRenderTargetView(),
					crystal_value_ptr(color));
			}
		}

		int clearFlag = 0;
		if (options & ClearOptions::CRYSTAL_CLEAR_DEPTH)
		{
			clearFlag |= D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH;
		}
		if (options & ClearOptions::CRYSTAL_CLEAR_STENCIL)
		{
			clearFlag |= D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL;
		}
		if (clearFlag)
		{
			if (m_renderTargetStackPtr == 0)
			{
				m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), clearFlag, depth, stencil);
			}
			else
			{
				auto ptr = m_renderTarget2DStack[m_renderTargetStackPtr]->GetDepthStencilView();
				if (ptr)
				{
					m_pd3dImmediateContext->ClearDepthStencilView(ptr,
						 clearFlag, depth, stencil);
				}
			}
			
		}
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
		varb.Format = GraphicsCommons::StringToComponentFormatConvert(type);
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

	std::shared_ptr<Texture2D> DX11GraphicsDevice::CreateTexture2DFromFile(const std::string& path, const Texture2DDescription& texDesc)
	{
		return std::make_shared<Texture2D>(this, path, texDesc);
	}

	std::shared_ptr<Texture2D> DX11GraphicsDevice::CreateTexture2DFromMemory(const uint8_t* src, size_t size, const Texture2DDescription& texDesc)
	{
		return std::make_shared<Texture2D>(this, src, size, texDesc);
	}

	std::shared_ptr<RenderTarget2D> DX11GraphicsDevice::CreateRenderTarget2D(const RenderTarget2DDescription& desc)
	{
		return std::make_shared<DX11RenderTarget2D>(this, desc);
		
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

	void DX11GraphicsDevice::PushRenderTarget2D(std::shared_ptr<RenderTarget2D> renderTarget2D)
	{
		assert(m_renderTargetStackPtr >= 0 && m_renderTargetStackPtr < NUM_RENDERTARGETS);
		m_renderTarget2DStack[++m_renderTargetStackPtr] = renderTarget2D;

		renderTarget2D->m_setToCurrentContext();
	}

	void DX11GraphicsDevice::PopRenderTarget2D()
	{
		assert(m_renderTargetStackPtr > 0 && m_renderTargetStackPtr < NUM_RENDERTARGETS);
		--m_renderTargetStackPtr;
		if (m_renderTargetStackPtr == 0)
		{
			m_pd3dImmediateContext->RSSetViewports(1, &m_ScreenViewport);
			m_pd3dImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
			return;
		}
		m_renderTarget2DStack[m_renderTargetStackPtr]->m_setToCurrentContext();
	}

	void DX11GraphicsDevice::PushPipelineStateObject(std::shared_ptr<PipelineStateObject> pso)
	{
		assert(m_PSOStackPtr >= 0 && m_PSOStackPtr < NUM_PIPELINE_STATE_OBJECTS - 1);
		auto& prevPSO = m_PSOStack[m_PSOStackPtr];
		m_PSOStack[++m_PSOStackPtr] = pso;

		pso->Apply();
	}

	void DX11GraphicsDevice::PopPipelineStateObject()
	{
		assert(m_PSOStackPtr > 0 && m_PSOStackPtr < NUM_PIPELINE_STATE_OBJECTS);

		auto& prevPSO = m_PSOStack[m_PSOStackPtr - 1];
		prevPSO->Apply();
		m_PSOStackPtr--;
	}

	void DX11GraphicsDevice::BindShaderResource(std::shared_ptr<IShaderResource> shaderResource, int index)
	{}

	void DX11GraphicsDevice::BindSamplerState(std::shared_ptr<SamplerState> texture, int index)
	{}


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

			m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.ReleaseAndGetAddressOf()));
			m_pd3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.ReleaseAndGetAddressOf());

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
		HR(m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.ReleaseAndGetAddressOf()));
		HR(m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.ReleaseAndGetAddressOf()));
	
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

	std::shared_ptr<SamplerState> DX11GraphicsDevice::GetCommonSamplerState(SamplerStates state)
	{
		switch (state)
		{
		case crystal::SamplerStates::PointClamp:
		{
			return m_commonStates->PointClamp;
		}
		break;
		case crystal::SamplerStates::PointWarp:
		{
			return m_commonStates->PointWarp;
		}
		break;
		case crystal::SamplerStates::LinearClamp:
		{
			return m_commonStates->LinearClamp;
		}
		break;
		case crystal::SamplerStates::LinearWarp:
		{
			return m_commonStates->LinearWarp;
		}
		break;
		default:
			break;
		}
		throw std::exception("Unknown Sampler State");
	}

	std::shared_ptr<BlendState> DX11GraphicsDevice::GetCommonBlendState(BlendStates state)
	{
		switch (state)
		{
		case crystal::BlendStates::Opaque:
		{
			return m_commonStates->Blend_Opaque;
		}
		break;
		case crystal::BlendStates::AlphaBlend:
		{
			return m_commonStates->Blend_Alpha;
		}
		break;
		case crystal::BlendStates::Additive:
		{
			return m_commonStates->Blend_Additive;
		}
		break;
		default:
			break;
		}
		throw std::exception("Unknown Blend State");
	}


	DX11GraphicsDevice::CommonStates::CommonStates(DX11GraphicsDevice* graphicsDevice)
		: m_pGraphicsDevice(graphicsDevice)
	{
		auto device = m_pGraphicsDevice->GetD3DDevice();
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
		HR(device->CreateSamplerState(&sampDesc, pointClamp.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(pointClamp.Get(), "Point Clamp Sampler");
		PointClamp = std::make_shared<SamplerState>(graphicsDevice, pointClamp);

		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		HR(device->CreateSamplerState(&sampDesc, pointWarp.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(pointWarp.Get(), "Point Warp Sampler");
		PointWarp = std::make_shared<SamplerState>(graphicsDevice, pointWarp);

		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		HR(device->CreateSamplerState(&sampDesc, linearWarp.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(linearWarp.Get(), "Linear Warp Sampler");
		LinearWarp = std::make_shared<SamplerState>(graphicsDevice, linearWarp);

		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		HR(device->CreateSamplerState(&sampDesc, linearClamp.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(linearClamp.Get(), "Linear Clamp Sampler");
		LinearClamp = std::make_shared<SamplerState>(graphicsDevice, linearClamp);


		ComPtr<ID3D11BlendState> blendOpaque;
		ComPtr<ID3D11BlendState> blendAlpha;
		ComPtr<ID3D11BlendState> blendAdditive;

		// Opaque, no blending
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = false;
		HR(device->CreateBlendState(&blendDesc, blendOpaque.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(blendOpaque.Get(), "Blend Opaque");
		Blend_Opaque = std::make_shared<BlendState>(this, blendOpaque);

		// Alpha Blending	src * alpha_s + dest * (1 - alpha_s)
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HR(device->CreateBlendState(&blendDesc, blendAlpha.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(blendAlpha.Get(), "Blend Alpha");
		Blend_Alpha = std::make_shared<BlendState>(this, blendAlpha);

		// Additive Blending  src + dest
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		HR(device->CreateBlendState(&blendDesc, blendAdditive.GetAddressOf()));
		d3dUtils::D3D11SetDebugObjectName(blendAdditive.Get(), "Blend Additive");
		Blend_Additive = std::make_shared<BlendState>(this, blendAdditive);
	}
}
