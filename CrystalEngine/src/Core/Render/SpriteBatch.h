#pragma once
#include <Core/Platform/Graphics/GraphicsCommon.h>
#include <Core/Utils/Geometry.h>
namespace crystal
{
	struct BatchVertex2D
	{
		Vector2f	Position;
		Vector2f	TextureCoords;
		Vector4f	Color;
		Float		TextureIndex;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch(IGraphicsDevice* graphicsDevice);
		~SpriteBatch();

		void Begin(const Matrix4f& transform);
		void End();

		void Draw(std::shared_ptr<Texture2D> texture, const Vector2f& center);
	private:
		std::shared_ptr<IVertexBuffer>	m_pVertexBuffer = nullptr;
		std::shared_ptr<IShaderProgram> m_pShaderProgram = nullptr;
		IGraphicsDevice*				m_pGraphicsDevice = nullptr;

		Matrix4f						m_renderMatrix{};

		std::map<Texture2D*, int>		m_textureSlotMap{};

		static constexpr int MAX_TEXTURE_SLOTS = 8;
		static constexpr int MAX_QUADS_PER_BATCH = 1 << 20;
		static constexpr int MAX_VERTICES_PER_BATCH = MAX_QUADS_PER_BATCH << 2;
		std::vector<BatchVertex2D>		m_vertices{};
		size_t							m_currentQuadIndex = 0;
		size_t							m_currentTextureSlot = 0;
		bool							m_batchHasBegin = false;

		// May be interface?
		std::shared_ptr<PipelineStateObject>	m_pPipelineStateObject = nullptr;
	};
}