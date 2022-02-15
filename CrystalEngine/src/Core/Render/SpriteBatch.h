#pragma once
#include <Interfaces/Graphics/GraphicsCommon.h>
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

	enum class SpriteSortMode
	{
		Deferred,
		Immediate,
		Texture,
		FrontToBack,
		BackToFront
	};

	class SpriteBatch
	{
	public:
		SpriteBatch(GraphicsDevice* graphicsDevice);
		~SpriteBatch();

		void Begin();
		void Begin(const Matrix4f& transform);
		void End();

		void Draw(std::shared_ptr<Texture2D> texture, const Vector2f& center, const Color4f& color);
		void Draw(std::shared_ptr<Texture2D> texture, const Bound2i& drawRect, const Color4f& color);
		void Draw(std::shared_ptr<Texture2D> texture, const Vector2f& pos, const Color4f& color,
			float rotation, const Vector2f& origin, float scale, SpriteEffect effect);
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}