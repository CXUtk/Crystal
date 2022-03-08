#pragma once
#include "RenderCommon.h"
#include <Core/Asset/Content/Font.h>

namespace crystal
{
	class SpriteBatch
	{
	public:
		SpriteBatch(IGraphicsDevice* graphicsDevice, IGraphicsContext* graphicsContext);
		~SpriteBatch();

		void Begin();
		void Begin(SpriteSortMode spriteSortMode,
            std::shared_ptr<IPipelineStateObject> PSO);
        void Begin(SpriteSortMode spriteSortMode,
            std::shared_ptr<IPipelineStateObject> PSO,
            std::shared_ptr<ISamplerState> samplerState);
        void Begin(SpriteSortMode spriteSortMode,
            std::shared_ptr<IPipelineStateObject> PSO,
            std::shared_ptr<ISamplerState> samplerState,
            std::shared_ptr<IShaderProgram> shader);
        void Begin(SpriteSortMode spriteSortMode,
            std::shared_ptr<IPipelineStateObject> PSO,
            std::shared_ptr<ISamplerState> samplerState,
            std::shared_ptr<IShaderProgram> shader,
            const Matrix4f& transform);
		void End();

		void Draw(std::shared_ptr<ITexture2D> texture, const Vector2f& center, const Color4f& color);
		void Draw(std::shared_ptr<ITexture2D> texture, const Bound2i& drawRect, const Color4f& color);
        void Draw(std::shared_ptr<ITexture2D> texture, const Bound2i& drawRect, const Bound2i* srcRect, const Color4f& color);
		void Draw(std::shared_ptr<ITexture2D> texture, const Vector2f& pos, const Color4f& color,
			float rotation, const Vector2f& origin, float scale, SpriteEffect effect);

        void DrawSlicedTexture(std::shared_ptr<ITexture2D> texture,
            const SliceInfo& sliceInfo, const Bound2i& screenBound, const Color4f& color);

        void DrawString(std::shared_ptr<Font> font, const std::string& text, const Vector2f& pos, const Color4f& color);
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}