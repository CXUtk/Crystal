#include "../GraphicsCommon.h"

namespace crystal
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = 0 {};	

		virtual void BindVertexLayout(const VertexLayout& layout) = 0;
		virtual void ChangeBufferContent(const void* src, size_t size, size_t offset) = 0;
	};
}