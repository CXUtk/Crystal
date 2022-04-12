#pragma once
#include "DX11Common.h"

namespace crystal
{
	class Win32GameWindow;

	class DX11GraphicsDevice : public IGraphicsDevice
	{
	public:
		DX11GraphicsDevice(Win32GameWindow* window);
		~DX11GraphicsDevice() override;

		ID3D11Device* GetD3DDevice() const { return m_pd3dDevice.Get(); }
		ComPtr<ID3D11Buffer> CreateBuffer(void* src, size_t size,
			BufferUsage usage, UINT bindFlags);

		virtual std::shared_ptr<IGraphicsContext> GetContext() override;

		virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const VertexBufferDescription& desc, void* src, size_t size) override;
		virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer(const IndexBufferDescription& desc, void* src, size_t size) override;
		virtual std::shared_ptr<IVertexShader> CreateVertexShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) override;
		virtual std::shared_ptr<IFragmentShader> CreateFragmentShaderFromMemory(const char* src, size_t size,
			const std::string& name, const std::string& entryPoint) override;
		virtual std::shared_ptr<IShaderProgram> CreateShaderProgram(std::shared_ptr<IVertexShader> vertexShader,
			std::shared_ptr<IFragmentShader> fragmentShader, const UniformVariableCollection& variables) override;
		virtual std::shared_ptr<ITexture2D> CreateTexture2DFromFile(const std::string& path, const Texture2DDescription& texDesc) override;
		virtual std::shared_ptr<ITexture2D> CreateTexture2DFromMemory(const uint8_t* src, size_t size, const Texture2DDescription& texDesc) override;
		virtual std::shared_ptr<IRenderTarget2D> CreateRenderTarget2D(const RenderTarget2DDescription& desc) override;

		virtual std::shared_ptr<IRasterState> CreateRasterState(const RasterStateDescription& rasterDesc) override;
		virtual std::shared_ptr<IBlendState> CreateBlendState(const BlendStateDescription& blendDesc) override;
		virtual std::shared_ptr<IDepthStencilState> CreateDepthStencilState(const DepthStencilStateDescription& DSSDesc) override;

		virtual std::shared_ptr<IPipelineStateObject> CreatePipelineStateObject() override;
		virtual std::shared_ptr<IPipelineResourceObject> CreatePipelineResourceObject() override;

        virtual std::shared_ptr<ISamplerState> GetCommonSamplerState(SamplerStates state) override;
        virtual std::shared_ptr<IBlendState> CreateBlendStateFromTemplate(BlendStates state) override;
        virtual std::shared_ptr<IDepthStencilState> CreateDepthStencilStateFromTemplate(DepthStencilStates state) override;
        virtual std::shared_ptr<IRasterState> CreateRasterStateFromTemplate(RasterStates state) override;
	private:
		std::shared_ptr<DX11GraphicsContext>	m_pGraphicsContext = nullptr;

		ComPtr<ID3D11Device>					m_pd3dDevice = nullptr;
		Win32GameWindow*						m_pWindow = nullptr;			

		class CommonStates;
		std::unique_ptr<CommonStates>			m_commonStates = nullptr;

		bool m_InitD3DX11();
		ComPtr<ID3DBlob> m_GetShaderBlobFromMemory(const char* src, size_t size, 
			const std::string& name, const std::string& entryPoint, ShaderType type);
	};
}