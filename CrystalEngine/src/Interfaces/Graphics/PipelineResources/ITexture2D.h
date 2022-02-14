#include "../GraphicsCommon.h"
#include "IShaderResource.h"

namespace crystal
{
	class ITexture2D : public IShaderReource
	{
	public:
		virtual ~ITexture2D() = 0 {};
	};
}