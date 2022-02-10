#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Core/Platform/Platforms.h>

namespace crystal
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	class IDX11ShaderResource
	{
	public:
		virtual ~IDX11ShaderResource() = 0 {};

		virtual ID3D11ShaderResourceView* GetShaderResourceView() const = 0;
	};

	class DX11Common{
	public:
		static void InitDX11Commons(DX11GraphicsDevice* graphicsDevice);

		static const char* RenderFormatToShaderVarConvert(RenderFormat format);
		static DXGI_FORMAT RenderFormatConvert(RenderFormat format, bool normalized);
		static size_t RenderFormatToBytes(RenderFormat format);
		static DXGI_FORMAT DataFormatConvert(DataFormat format);
		static D3D11_USAGE BufferUsageToDX11Convert(BufferUsage usage);
		static const char* SemanticNameConvert(SemanticType semanticType);
		static const char* ShaderModelConvert(ShaderType shaderType);
		static D3D11_PRIMITIVE_TOPOLOGY PrimitiveTypeToTopologyConvert(PrimitiveType type);

		static D3D11_CULL_MODE CullModeConvert(CullingMode mode);
		static D3D11_FILL_MODE FillModeConvert(FillMode mode);

		static std::string ConvertFromUtf16ToUtf8(const std::wstring& wstr)
		{
			std::string convertedString;
			int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
			if (requiredSize > 0)
			{
				std::vector<char> buffer(requiredSize);
				WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
				convertedString.assign(buffer.begin(), buffer.end() - 1);
			}
			return convertedString;
		}

		static std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
		{
			std::wstring convertedString;
			int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
			if (requiredSize > 0)
			{
				std::vector<wchar_t> buffer(requiredSize);
				MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
				convertedString.assign(buffer.begin(), buffer.end() - 1);
			}
			return convertedString;
		}
	};
}