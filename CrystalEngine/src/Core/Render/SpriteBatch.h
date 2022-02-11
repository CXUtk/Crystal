#pragma once
#include <Core/Platform/Graphics/GraphicsCommon.h>
#include <Core/Utils/Geometry.h>
#include "RenderCommon.h"

namespace crystal
{
	enum SpriteEffect : int
	{
		CRYSTAL_SPRITEEFFECT_NONE = 0,
		CRYSTAL_SPRITEEFFECT_FLIP_HORIZONTAL = 1 << 0,
		CRYSTAL_SPRITEEFFECT_FLIP_VERTICAL = 1 << 1,
	};

	inline SpriteEffect operator|(SpriteEffect a, SpriteEffect b)
	{
		return static_cast<SpriteEffect>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline SpriteEffect operator&(SpriteEffect a, SpriteEffect b)
	{
		return static_cast<SpriteEffect>(static_cast<int>(a) & static_cast<int>(b));
	}

	class SpriteBatch
	{
	public:
		SpriteBatch(IGraphicsDevice* graphicsDevice);
		~SpriteBatch();

		void PushBatch(const Matrix4f& transform);
		void PopBatch();

		void Draw(std::shared_ptr<Texture2D> texture, const Vector2f& center, const Color4f& color);
		void Draw(std::shared_ptr<Texture2D> texture, const Bound2i& drawRect, const Color4f& color);
		void Draw(std::shared_ptr<Texture2D> texture, const Vector2f& pos, const Color4f& color,
			float rotation, const Vector2f& origin, float scale, SpriteEffect effect);

		bool BatchNotEmpty() const { return !m_renderStateStack.empty(); }

	private:
		std::shared_ptr<IVertexBuffer>	m_pDefaultVertexBuffer = nullptr;
		std::shared_ptr<IShaderProgram> m_pDefaultShaderProgram = nullptr;
		IGraphicsDevice*				m_pGraphicsDevice = nullptr;

		class RenderState;
		std::shared_ptr<RenderState>		m_pDefaultRenderState = nullptr;
		std::vector<RenderState>			m_renderStateStack{};

		static constexpr int MAX_TEXTURE_SLOTS = 8;
		static constexpr int MAX_QUADS_PER_BATCH = 1 << 20;
		static constexpr int MAX_VERTICES_PER_BATCH = MAX_QUADS_PER_BATCH << 2;

		struct BatchVertex2D;
		std::vector<BatchVertex2D>		m_vertices{};
		size_t							m_globalQuadIndex = 0;
		size_t							m_currentVertexIndex = 0;

		static BatchVertex2D			m_defaultVertices[4];

		int m_GetTextureSlot(std::shared_ptr<Texture2D> texture);
		void m_FlushThisBatch();
		void m_UpdateOneQuad();
		void m_CheckFlush();
		void m_PushOneVertex(const BatchVertex2D& vertex);

		void m_GeneralDraw(std::shared_ptr<Texture2D> texture, const Vector2f& pos,
			const Bound2i& srcRect, const Color4f& color, float rotation, const Vector2f& origin, 
			const Vector2f& scale, SpriteEffect effect, float depth);
	};
}