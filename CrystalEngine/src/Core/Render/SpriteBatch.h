#pragma once
#include <Core/Platform/Graphics/GraphicsCommon.h>

namespace crystal
{
	class SpriteBatch
	{
	public:
		SpriteBatch(IGraphicsDevice* graphicsDevice);
		~SpriteBatch();


	private:
		std::shared_ptr<VertexBuffer>	m_vertexBuffer;
		std::shared_ptr<IndexBuffer>	m_indexBuffer;
	};
}