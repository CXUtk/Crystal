#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Core/Platform/Graphics/IGraphicsDevice.h>
#include <Core/Platform/Graphics/IShaders.h>

namespace crystal
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	class DX11GraphicsDevice;
	class DX11Provider;

	class DX11VertexShader;
	class DX11FragmentShader;
	class DX11VertexBuffer;

	void InitDX11Commons();

	const char* VertexElementFormatToShaderVarConvert(VertexElementFormat format);
	DXGI_FORMAT VertexElementFormatConvert(VertexElementFormat format);
	DXGI_FORMAT DataFormatConvert(DataFormat format);
	D3D11_USAGE BufferUsageToDX11Convert(BufferUsage usage);
	const char* SemanticNameConvert(SemanticType semanticType);
	const char* ShaderModelConvert(ShaderType shaderType);
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTypeToTopologyConvert(PrimitiveType type);
	ComponentFormat StringToComponentFormatConvert(const std::string& type);
	size_t ComponentFormatToSizeConvert(ComponentFormat format);
}