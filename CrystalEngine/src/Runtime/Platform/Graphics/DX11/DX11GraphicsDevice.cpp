#include "DX11GraphicsDevice.h"
#include "d3dUtils.h"
#include "dxTrace.h"

#include "DX11PipelineStateObject.h"
#include "DX11PipelineResourceObject.h"

#include "PipelineResources/DX11VertexBuffer.h"
#include "PipelineResources/DX11IndexBuffer.h"
#include "PipelineResources/DX11VertexShader.h"
#include "PipelineResources/DX11FragmentShader.h"
#include "PipelineResources/DX11ShaderProgram.h"
#include "PipelineResources/DX11Texture2D.h"
#include "PipelineResources/DX11TextureCubemap.h"
#include "PipelineResources/DX11RenderTarget2D.h"
#include "PipelineResources/DX11SamplerState.h"
#include "PipelineStates/DX11BlendState.h"
#include "PipelineStates/DX11RasterState.h"
#include "PipelineStates/DX11DepthStencilState.h"
#include "DX11GraphicsContext.h"

#include "Engine.h"
#include "Resource/Config/ConfigManager.h"

#include <Core/Utils/Misc.h>
#include <Core/Utils/IO.h>
#include <glm/gtx/quaternion.hpp>

namespace crystal
{

    class DX11GraphicsDevice::CommonStates
    {
    public:
        CommonStates(DX11GraphicsDevice* graphicsDevice);

        std::shared_ptr<ISamplerState> PointClamp = nullptr;
        std::shared_ptr<ISamplerState> PointWarp = nullptr;
        std::shared_ptr<ISamplerState> LinearClamp = nullptr;
        std::shared_ptr<ISamplerState> LinearWarp = nullptr;


        BlendStateDescription		        Blend_Opaque{};
        BlendStateDescription		        Blend_Alpha{};
        BlendStateDescription		        Blend_Additive{};

        DepthStencilStateDescription	    NoDepthTest{};
        DepthStencilStateDescription	    DefaultDepthTest{};
        
        RasterStateDescription		        Raster_CullNone{};
        RasterStateDescription		        Raster_CullCCW{};
        RasterStateDescription		        Raster_CullCW{};

    private:
        DX11GraphicsDevice*		m_pGraphicsDevice;
    };

    DX11GraphicsDevice::DX11GraphicsDevice( Win32GameWindow* window)
        : m_pWindow(window)
    {
        if (!m_InitD3DX11())
        {
            throw std::exception("[DX11GraphicsDevice::DX11GraphicsDevice] Unable to start Dx11");
        }
        m_commonStates = std::make_unique<CommonStates>(this);
    }

    DX11GraphicsDevice::~DX11GraphicsDevice()
    {}

    std::shared_ptr<IGraphicsContext> DX11GraphicsDevice::GetContext()
    {
        return m_pGraphicsContext;
    }

    std::shared_ptr<DX11GraphicsContext> DX11GraphicsDevice::GetD3DContext()
    {
        return m_pGraphicsContext;
    }

    std::shared_ptr<IPipelineStateObject> DX11GraphicsDevice::CreatePipelineStateObject()
    {
        return std::make_shared<DX11PipelineStateObject>(this, ptr(m_pGraphicsContext));
    }

    std::shared_ptr<IPipelineResourceObject> DX11GraphicsDevice::CreatePipelineResourceObject()
    {
        return std::make_shared<DX11PipelineResourceObject>(this, ptr(m_pGraphicsContext));
    }

    std::shared_ptr<IVertexBuffer> DX11GraphicsDevice:: CreateVertexBuffer(const VertexBufferDescription& desc, void* src, size_t size)
    {
        ComPtr<ID3D11Buffer> vertexBuffer = CreateBuffer(src, size, desc.Usage, D3D11_BIND_VERTEX_BUFFER);
        return std::make_shared<DX11VertexBuffer>(this, vertexBuffer);
    }

    std::shared_ptr<IIndexBuffer> DX11GraphicsDevice::CreateIndexBuffer(const IndexBufferDescription& desc, 
        void* src, size_t size)
    {
        ComPtr<ID3D11Buffer> indexBuffer = CreateBuffer(src, size, desc.Usage, D3D11_BIND_INDEX_BUFFER);
        return std::make_shared<DX11IndexBuffer>(this, indexBuffer, DX11Common::DataFormatConvert(desc.Format));
    }

    std::shared_ptr<IVertexShader> DX11GraphicsDevice::CreateVertexShaderFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint)
    {
        auto pBlob = m_GetShaderBlobFromMemory(src, size, name, entryPoint, ShaderType::VERTEX_SHADER);
        ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
        HR(m_pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
            nullptr, pVertexShader.GetAddressOf()));
        return std::make_shared<DX11VertexShader>(this, pVertexShader);
    }

    std::shared_ptr<IFragmentShader> DX11GraphicsDevice::CreateFragmentShaderFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint)
    {
        auto pBlob = m_GetShaderBlobFromMemory(src, size, name, entryPoint, ShaderType::FRAGMENT_SHADER);
        ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
        HR(m_pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
            nullptr, pPixelShader.GetAddressOf()));
        return std::make_shared<DX11FragmentShader>(this, pPixelShader);
    }

    std::shared_ptr<IShaderProgram> DX11GraphicsDevice::CreateShaderProgram(std::shared_ptr<IVertexShader> vertexShader,
            std::shared_ptr<IFragmentShader> fragmentShader, const UniformVariableCollection& variables)
    {
        return std::make_shared<DX11ShaderProgram>(this, vertexShader, 
            fragmentShader, variables);
    }

    std::shared_ptr<ITexture2D> DX11GraphicsDevice::CreateTexture2DFromFile(const std::string& path, const Texture2DDescription& texDesc)
    {
        return std::make_shared<DX11Texture2D>(this, path, texDesc);
    }

    std::shared_ptr<ITexture2D> DX11GraphicsDevice::CreateTexture2DFromMemory(const void* src, size_t size, const Texture2DDescription& texDesc)
    {
        return std::make_shared<DX11Texture2D>(this, src, size, texDesc);
    }

    std::shared_ptr<ITextureCubemap> DX11GraphicsDevice::CreateCubemapFromTexture6(const CubemapTexture6& tex6, const Texture2DDescription& texDesc)
    {
        return std::make_shared<DX11TextureCubemap>(this, tex6, texDesc);
    }

    std::shared_ptr<IRenderTarget2D> DX11GraphicsDevice::CreateRenderTarget2D(const RenderTarget2DDescription& desc)
    {
        return std::make_shared<DX11RenderTarget2D>(this, desc);	
    }

    std::shared_ptr<IRasterState> DX11GraphicsDevice::CreateRasterState(const RasterStateDescription& rasterDesc)
    {
        return std::make_shared<DX11RasterState>(this, rasterDesc);
    }

    std::shared_ptr<IBlendState> DX11GraphicsDevice::CreateBlendState(const BlendStateDescription& blendDesc)
    {
        return std::make_shared<DX11BlendState>(this, blendDesc);
    }

    std::shared_ptr<IDepthStencilState> DX11GraphicsDevice::CreateDepthStencilState(const DepthStencilStateDescription& DSSDesc)
    {
        return std::make_shared<DX11DepthStencilState>(this, DSSDesc);
    }

    bool DX11GraphicsDevice::m_InitD3DX11()
    {
        const auto& args = Engine::GetInstance()->GetConfigManager()->GetAppInitArgs();
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

        m_pGraphicsContext = std::make_shared<DX11GraphicsContext>(this, m_pWindow, immediateContext);
        return true;
    }

    ComPtr<ID3DBlob> DX11GraphicsDevice::m_GetShaderBlobFromMemory(const char* src, size_t size, const std::string& name, const std::string& entryPoint, ShaderType type)
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

    std::shared_ptr<ISamplerState> DX11GraphicsDevice::GetCommonSamplerState(SamplerStates state)
    {
        switch (state)
        {
        case crystal::SamplerStates::PointClamp:
        {
            return m_commonStates->PointClamp;
        }
        case crystal::SamplerStates::PointWarp:
        {
            return m_commonStates->PointWarp;
        }
        case crystal::SamplerStates::LinearClamp:
        {
            return m_commonStates->LinearClamp;
        }
        case crystal::SamplerStates::LinearWarp:
        {
            return m_commonStates->LinearWarp;
        }
        default:
            break;
        }
        throw std::exception("Unknown Sampler State");
    }

    std::shared_ptr<IBlendState> DX11GraphicsDevice::CreateBlendStateFromTemplate(BlendStates state)
    {
        switch (state)
        {
        case crystal::BlendStates::Opaque:
        {
            return CreateBlendState(m_commonStates->Blend_Opaque);
        }
        case crystal::BlendStates::AlphaBlend:
        {
            return CreateBlendState(m_commonStates->Blend_Alpha);
        }
        case crystal::BlendStates::Additive:
        {
            return CreateBlendState(m_commonStates->Blend_Additive);
        }
        default:
            break;
        }
        throw std::exception("DX11GraphicsDevice: Unknown Blend State");
    }

    std::shared_ptr<IDepthStencilState> DX11GraphicsDevice::CreateDepthStencilStateFromTemplate(DepthStencilStates state)
    {
        switch (state)
        {
        case crystal::DepthStencilStates::NoDepthTest:
        {
            return CreateDepthStencilState(m_commonStates->NoDepthTest);
        }
        case crystal::DepthStencilStates::DefaultDepthTest:
        {
            return CreateDepthStencilState(m_commonStates->DefaultDepthTest);
        }
        default:
            break;
        }
        throw std::exception("DX11GraphicsDevice: Unknown Depth Stencil State");
    }

    std::shared_ptr<IRasterState> DX11GraphicsDevice::CreateRasterStateFromTemplate(RasterStates state)
    {
        switch (state)
        {
        case crystal::RasterStates::CullNone:
        {
            return CreateRasterState(m_commonStates->Raster_CullNone);
        }
        case crystal::RasterStates::CullCW:
        {
            return CreateRasterState(m_commonStates->Raster_CullCW);
        }
        case crystal::RasterStates::CullCCW:
        {
            return CreateRasterState(m_commonStates->Raster_CullCCW);
        }
        default:
            break;
        }
        throw std::exception("Unknown Raster State");
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
        PointClamp = std::make_shared<DX11SamplerState>(graphicsDevice, pointClamp);

        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
        HR(device->CreateSamplerState(&sampDesc, pointWarp.GetAddressOf()));
        d3dUtils::D3D11SetDebugObjectName(pointWarp.Get(), "Point Warp Sampler");
        PointWarp = std::make_shared<DX11SamplerState>(graphicsDevice, pointWarp);

        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        HR(device->CreateSamplerState(&sampDesc, linearWarp.GetAddressOf()));
        d3dUtils::D3D11SetDebugObjectName(linearWarp.Get(), "Linear Warp Sampler");
        LinearWarp = std::make_shared<DX11SamplerState>(graphicsDevice, linearWarp);

        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
        HR(device->CreateSamplerState(&sampDesc, linearClamp.GetAddressOf()));
        d3dUtils::D3D11SetDebugObjectName(linearClamp.Get(), "Linear Clamp Sampler");
        LinearClamp = std::make_shared<DX11SamplerState>(graphicsDevice, linearClamp);


        // Opaque, no blending
        BlendStateDescription blendDesc = {};
        blendDesc.EnableBlending = true;
        blendDesc.SrcBlend = BlendFactors::One;
        blendDesc.DestBlend = BlendFactors::Zero;
        blendDesc.BlendOp = BlendOperations::Add;
        blendDesc.SrcBlendAlpha = BlendFactors::One;
        blendDesc.DestBlendAlpha = BlendFactors::Zero;
        blendDesc.RenderTargetWriteMask = 0xF;
        Blend_Opaque = blendDesc;

        // Alpha Blending	src * alpha_s + dest * (1 - alpha_s)
        blendDesc.SrcBlend = BlendFactors::SrcAlpha;
        blendDesc.DestBlend = BlendFactors::InvSrcAlpha;
        Blend_Alpha = blendDesc;

        // Additive Blending  src + dest
        blendDesc.SrcBlend = BlendFactors::One;
        blendDesc.DestBlend = BlendFactors::One;
        Blend_Additive = blendDesc;


        DepthStencilStateDescription DSDesc = {};
        DSDesc.EnableDepthTest = false;
        DSDesc.EnableStencilTest = false;
        NoDepthTest = DSDesc;

        DSDesc.EnableDepthTest = true;
        DSDesc.EnableDepthWrite = true;
        DSDesc.DepthFunction = ComparisonFunction::LessEqual;
        DSDesc.StencilReadMask = 0xFF;
        DSDesc.StencilWriteMask = 0xFF;
        DefaultDepthTest = DSDesc;


        RasterStateDescription rasterDesc = {};
        rasterDesc.CullMode = CullingMode::None;
        rasterDesc.FillMode = FillMode::SOLID;
        rasterDesc.EnableScissorTest = false;
        rasterDesc.EnableAntialiasedLine = false;
        Raster_CullNone = rasterDesc;

        rasterDesc.CullMode = CullingMode::CullCW;
        Raster_CullCW = rasterDesc;

        rasterDesc.CullMode = CullingMode::CullCCW;
        Raster_CullCCW = rasterDesc;
    }
}
