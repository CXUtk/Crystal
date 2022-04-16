#include "DX11TextureCubemap.h"

#include "../DX11GraphicsContext.h"
#include "../DX11GraphicsDevice.h"
#include "../WICTextureLoader.h"
#include "../dxTrace.h"

namespace crystal
{
    DX11TextureCubemap::DX11TextureCubemap(DX11GraphicsDevice* graphicsDevice,
        const CubemapTexture6& texture6, const Texture2DDescription& texDesc)
        : m_pGraphicsDevice(graphicsDevice)
    {
        D3D11_TEXTURE2D_DESC cubeTexDesc = {};
        cubeTexDesc.Format = DX11Common::RenderFormatConvert(texDesc.Format, true);
        cubeTexDesc.Width = texDesc.Size.x;
        cubeTexDesc.Height = texDesc.Size.y;
        cubeTexDesc.MipLevels = texDesc.MipmapLevels;

        // Cubemap has 6 components
        cubeTexDesc.ArraySize = 6;
        cubeTexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        cubeTexDesc.Format = DX11Common::RenderFormatConvert(texDesc.Format, true);
        cubeTexDesc.Usage = DX11Common::BufferUsageToDX11Convert(texDesc.Usage);
        cubeTexDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        cubeTexDesc.CPUAccessFlags = 0;
        cubeTexDesc.SampleDesc.Count = 1;
        cubeTexDesc.SampleDesc.Quality = 0;

        if (texDesc.Usage == BufferUsage::CPURead)
        {
            cubeTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
        }
        else if (texDesc.Usage == BufferUsage::CPUWrite)
        {
            cubeTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
        }
        else if (texDesc.Usage == BufferUsage::CPURWrite)
        {
            cubeTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
        }

        //if (cubeTexDesc.MipLevels > 1)
        //{
        //    cubeTexDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
        //    cubeTexDesc.MiscFlags |= D3D10_RESOURCE_MISC_GENERATE_MIPS;
        //}

        auto device = m_pGraphicsDevice->GetD3DDevice();
        HR(device->CreateTexture2D(&cubeTexDesc, nullptr, m_pCubemapTexture.GetAddressOf()));

        //if (cubeTexDesc.MipLevels > 1)
        //{
        //    m_pGraphicsDevice->GetD3DContext()->GetD3DContext()->GenerateMips(m_pSRV.Get());
        //}

        auto deviceContext = m_pGraphicsDevice->GetD3DContext()->GetD3DContext();

        {
            auto texture = std::dynamic_pointer_cast<DX11Texture2D>(texture6.posX)->GetTexture2D();
            for (int j = 0; j < cubeTexDesc.MipLevels; ++j)
            {
                deviceContext->CopySubresourceRegion(
                    m_pCubemapTexture.Get(),
                    D3D11CalcSubresource(j, D3D11_TEXTURECUBE_FACE_POSITIVE_X, cubeTexDesc.MipLevels),
                    0, 0, 0,
                    texture,
                    j,
                    nullptr);
            }
        }

        {
            auto texture = std::dynamic_pointer_cast<DX11Texture2D>(texture6.negX)->GetTexture2D();
            for (int j = 0; j < cubeTexDesc.MipLevels; ++j)
            {
                deviceContext->CopySubresourceRegion(
                    m_pCubemapTexture.Get(),
                    D3D11CalcSubresource(j, D3D11_TEXTURECUBE_FACE_NEGATIVE_X, cubeTexDesc.MipLevels),
                    0, 0, 0,
                    texture,
                    j,
                    nullptr);
            }
        }

        {
            auto texture = std::dynamic_pointer_cast<DX11Texture2D>(texture6.posY)->GetTexture2D();
            for (int j = 0; j < cubeTexDesc.MipLevels; ++j)
            {
                deviceContext->CopySubresourceRegion(
                    m_pCubemapTexture.Get(),
                    D3D11CalcSubresource(j, D3D11_TEXTURECUBE_FACE_POSITIVE_Y, cubeTexDesc.MipLevels),
                    0, 0, 0,
                    texture,
                    j,
                    nullptr);
            }
        }

        {
            auto texture = std::dynamic_pointer_cast<DX11Texture2D>(texture6.negY)->GetTexture2D();
            for (int j = 0; j < cubeTexDesc.MipLevels; ++j)
            {
                deviceContext->CopySubresourceRegion(
                    m_pCubemapTexture.Get(),
                    D3D11CalcSubresource(j, D3D11_TEXTURECUBE_FACE_NEGATIVE_Y, cubeTexDesc.MipLevels),
                    0, 0, 0,
                    texture,
                    j,
                    nullptr);
            }
        }

        {
            auto texture = std::dynamic_pointer_cast<DX11Texture2D>(texture6.posZ)->GetTexture2D();
            for (int j = 0; j < cubeTexDesc.MipLevels; ++j)
            {
                deviceContext->CopySubresourceRegion(
                    m_pCubemapTexture.Get(),
                    D3D11CalcSubresource(j, D3D11_TEXTURECUBE_FACE_POSITIVE_Z, cubeTexDesc.MipLevels),
                    0, 0, 0,
                    texture,
                    j,
                    nullptr);
            }
        }

        {
            auto texture = std::dynamic_pointer_cast<DX11Texture2D>(texture6.negZ)->GetTexture2D();
            for (int j = 0; j < cubeTexDesc.MipLevels; ++j)
            {
                deviceContext->CopySubresourceRegion(
                    m_pCubemapTexture.Get(),
                    D3D11CalcSubresource(j, D3D11_TEXTURECUBE_FACE_NEGATIVE_Z, cubeTexDesc.MipLevels),
                    0, 0, 0,
                    texture,
                    j,
                    nullptr);
            }
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
        SRVDesc.Format = cubeTexDesc.Format;
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        SRVDesc.TextureCube.MipLevels = texDesc.MipmapLevels;
        SRVDesc.TextureCube.MostDetailedMip = 0;

        m_mipMapLevels = texDesc.MipmapLevels;

        HR(device->CreateShaderResourceView(m_pCubemapTexture.Get(), nullptr, m_pSRV.GetAddressOf()));
    }

    DX11TextureCubemap::~DX11TextureCubemap()
    {}

    void DX11TextureCubemap::GetShaderResourceHandle(void* pHandle) const
    {
        ID3D11ShaderResourceView** ptr = (ID3D11ShaderResourceView**)pHandle;
        *ptr = m_pSRV.Get();
    }

    void DX11TextureCubemap::ReplaceMipmap(std::shared_ptr<ITextureCubemap> mipCubemap, int mipLevel)
    {
        auto deviceContext = m_pGraphicsDevice->GetD3DContext()->GetD3DContext();
        auto dx11TextureCubemap = std::dynamic_pointer_cast<DX11TextureCubemap>(mipCubemap);

        for (size_t face = 0; face < 6; face++)
        {
            deviceContext->CopySubresourceRegion(
                m_pCubemapTexture.Get(),
                D3D11CalcSubresource(mipLevel, face, m_mipMapLevels),
                0, 0, 0,
                dx11TextureCubemap->m_pCubemapTexture.Get(),
                D3D11CalcSubresource(0, face, 1),
                nullptr);
        }

    }
}