#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Core/Platform/Graphics/IGraphicsDevice.h>

namespace crystal
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	class DX11GraphicsDevice;
	class DX11Provider;


	const char* VertexElementFormatToShaderVarConvert(VertexElementFormat format);
	DXGI_FORMAT VertexElementFormatConvert(VertexElementFormat format);
	const char* SemanticNameConvert(SemanticType semanticType);
	const char* ShaderModelConvert(ShaderType shaderType);
}