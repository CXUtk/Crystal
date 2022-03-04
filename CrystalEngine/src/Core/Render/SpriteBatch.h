#pragma once
#include "RenderCommon.h"
#include <Core/Asset/Content/Font.h>

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
		SpriteBatch(IGraphicsDevice* graphicsDevice, IGraphicsContext* graphicsContext);
		~SpriteBatch();

		void Begin();
		void Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState);
		void Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState,
			std::shared_ptr<IBlendState> blendState);
		void Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState,
			std::shared_ptr<IBlendState> blendState, std::shared_ptr<IShaderProgram> shader);
		void Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState,
			std::shared_ptr<IBlendState> blendState, std::shared_ptr<IShaderProgram> shader, 
			Matrix4f& transform);
		void Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState,
			std::shared_ptr<IBlendState> blendState, std::shared_ptr<IShaderProgram> shader,
			Matrix4f& transform, std::shared_ptr<IRasterState> rasterState,
			std::shared_ptr<IDepthStencilState> depthStencilState);
		void End();

		void Draw(std::shared_ptr<ITexture2D> texture, const Vector2f& center, const Color4f& color);
		void Draw(std::shared_ptr<ITexture2D> texture, const Bound2i& drawRect, const Color4f& color);
        void Draw(std::shared_ptr<ITexture2D> texture, const Bound2i& drawRect, const Bound2i* srcRect, const Color4f& color);
		void Draw(std::shared_ptr<ITexture2D> texture, const Vector2f& pos, const Color4f& color,
			float rotation, const Vector2f& origin, float scale, SpriteEffect effect);

        void DrawString(std::shared_ptr<Font> font, const std::string& text, const Vector2f& pos, const Color4f& color);
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};

    class SpriteBatchUtils
    {
    public:
        static void DrawNineSquareTexture(SpriteBatch* spriteBatch, std::shared_ptr<ITexture2D> texture,
            const Vector2i& cornerSize, const Bound2i& bound, const Color4f& color);
    };
}