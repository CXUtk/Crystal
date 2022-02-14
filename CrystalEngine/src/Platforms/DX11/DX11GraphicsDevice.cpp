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
#include "DX11GraphicsContext.h"
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
		: m_pWindow(window)
	{
		if (!m_InitD3DX11(args))
		{
			throw std::exception("[DX11GraphicsDevice::DX11GraphicsDevice] Unable to start Dx11");
		}

		m_PSOStack[0] = CreatePipelineStateObject();
		m_commonStates = std::make_unique<CommonStates>(this);
	}

	DX11GraphicsDevice::~DX11GraphicsDevice()
	{}

	std::shared_ptr<IGraphicsContext> DX11GraphicsDevice::GetContext()
	{
		return m_pGraphicsContext;
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


	bool DX11GraphicsDevice::m_InitD3DX11(const InitArgs& args)
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

		ComPtr<ID3D11DeviceContext> immediateContext;
		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(),
			&curFeatureLevel, immediateContext.GetAddressOf());

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

		m_pGraphicsContext = std::make_shared<DX11GraphicsContext>(this, m_pWindow, immediateContext, args);
		return true;
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
