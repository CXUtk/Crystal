#include "../GraphicsCommon.h"
#include "IShaderResource.h"

namespace crystal
{
	class IRenderTarget2D : public IShaderReources
	{
	public:
		virtual ~IRenderTarget2D() = 0 {};
	};
}