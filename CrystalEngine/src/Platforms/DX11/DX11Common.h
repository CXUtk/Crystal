#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Core/Platform/Platforms.h>

namespace crystal
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	class DX11GraphicsDevice;
	class DX11Provider;

	class DX11VertexShader;
	class DX11FragmentShader;
	class DX11VertexBuffer;
	class DX11ShaderProgram;

	class DX11Common{
	public:
		static void InitDX11Commons();

		static const char* VertexElementFormatToShaderVarConvert(RenderFormat format);
		static DXGI_FORMAT RenderFormatConvert(RenderFormat format);
		static DXGI_FORMAT DataFormatConvert(DataFormat format);
		static D3D11_USAGE BufferUsageToDX11Convert(BufferUsage usage);
		static const char* SemanticNameConvert(SemanticType semanticType);
		static const char* ShaderModelConvert(ShaderType shaderType);
		static D3D11_PRIMITIVE_TOPOLOGY PrimitiveTypeToTopologyConvert(PrimitiveType type);

		static D3D11_CULL_MODE CullModeConvert(CullingMode mode);
		static D3D11_FILL_MODE FillModeConvert(FillMode mode);
	};
}