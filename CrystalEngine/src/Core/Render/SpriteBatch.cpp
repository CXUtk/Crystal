#include "SpriteBatch.h"
#include <Interfaces/Graphics/GraphicsInterfaces.h>
#include <stdexcept>
#include <Engine.h>
#include <Core/Asset/AssetManager.h>


namespace crystal
{
	static constexpr int VERTICES_PER_QUAD = 4;
	static constexpr int INDICIES_PER_QUAD = 6;

	struct BatchVertex2D
	{
		Vector3f	Position;
		Vector2f	TextureCoords;
		Color4f		Color;
	};

	struct SpriteInfo
	{
		Bound2f							TexCoords;
		Bound2f							DestRect;
		Color4f							Color;
		Vector2f						Origin;
		float							Depth;
		float							Rotation;
		std::shared_ptr<ITexture2D>		Texture;
		SpriteEffect					SpriteEffect;
	};

	struct RenderState
	{
		SpriteSortMode							m_spriteSortMode = SpriteSortMode::Deferred;
		Matrix4f								m_renderMatrix{};
		std::shared_ptr<IShaderProgram>			m_pShaderProgram = nullptr;
		std::shared_ptr<IRasterState>			m_pRasterState = nullptr;
		std::shared_ptr<IBlendState>			m_pBlendState = nullptr;
		std::shared_ptr<IDepthStencilState>		m_pDepthStencilState = nullptr;
		std::shared_ptr<ISamplerState>			m_pSamplerState = nullptr;
	};

	class SpriteBatch::Impl
	{
	public:
		explicit Impl(IGraphicsDevice* graphicsDevice, IGraphicsContext* graphicsContext);
		~Impl();
		 

		void Begin(SpriteSortMode spriteSortMode,
			const Matrix4f* transform,
			std::shared_ptr<IBlendState> blendState,
			std::shared_ptr<ISamplerState> samplerState,
			std::shared_ptr<IShaderProgram> shader,
			std::shared_ptr<IRasterState> rasterState,
			std::shared_ptr<IDepthStencilState> depthStencilState);
		void End();

		void Draw(std::shared_ptr<ITexture2D> texture, const Bound2f& destRect,
					const Bound2i* srcRect, const Color4f& color, const Vector2f& origin, float rotation,
					SpriteEffect effect, float depth);
	private:
		static constexpr int MAX_QUADS_PER_BATCH = 1 << 12;
		static constexpr int MAX_VERTICES_PER_BATCH = MAX_QUADS_PER_BATCH << 2;

		IGraphicsDevice*					m_pGraphicsDevice = nullptr;
		IGraphicsContext*					m_pGraphicsContext = nullptr;

		std::shared_ptr<IPipelineStateObject>		m_pDefaultPSO = nullptr;
		std::shared_ptr<IPipelineResourceObject>	m_pDefaultPRO = nullptr;
		std::shared_ptr<IVertexBuffer>				m_pDefaultVertexBuffer = nullptr;

		RenderState						m_defaultRenderState{};
		RenderState						m_currentRenderState{};
		std::vector<BatchVertex2D>		m_vertices{};
		std::vector<SpriteInfo>			m_spriteQueue{};
		bool							m_isBatchingBegin = false;

		std::set<std::shared_ptr<ITexture2D>>	m_textureRefSet;
		static BatchVertex2D					m_defaultVertices[4];

		void m_FlushThisBatch();
		void m_RenderBatch(std::shared_ptr<ITexture2D> texture, const SpriteInfo* src, size_t count);
		void m_PushOneQuad(const SpriteInfo& sprite);
	};

	BatchVertex2D SpriteBatch::Impl::m_defaultVertices[] =
	{
		{ Vector3f(-0.5f, -0.5f, 0.f), Vector2f(0.0f, 0.0f), Vector4f(1.0f) },
		{ Vector3f(0.5f, -0.5f, 0.f), Vector2f(1.0f, 0.0f), Vector4f(1.0f) },
		{ Vector3f(0.5f, 0.5f, 0.f), Vector2f(1.0f, 1.0f), Vector4f(1.0f) },
		{ Vector3f(-0.5f, 0.5f, 0.f), Vector2f(0.0f, 1.0f), Vector4f(1.0f) },
	};

	SpriteBatch::SpriteBatch(IGraphicsDevice* graphicsDevice, IGraphicsContext* graphicsContext)
	{
		m_pImpl = std::make_unique<Impl>(graphicsDevice, graphicsContext);
	}

	SpriteBatch::~SpriteBatch()
	{}

	void SpriteBatch::Begin()
	{
		m_pImpl->Begin(SpriteSortMode::Deferred,
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	}

	void SpriteBatch::Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState)
	{
		m_pImpl->Begin(spriteSortMode, nullptr, nullptr, samplerState, nullptr,
				nullptr, nullptr);
	}

	void SpriteBatch::Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState, 
		std::shared_ptr<IBlendState> blendState)
	{
		m_pImpl->Begin(spriteSortMode, nullptr, blendState, samplerState, nullptr,
					nullptr, nullptr);
	}

	void SpriteBatch::Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState, 
		std::shared_ptr<IBlendState> blendState, std::shared_ptr<IShaderProgram> shader)
	{
		m_pImpl->Begin(spriteSortMode, nullptr, blendState, samplerState, shader,
			nullptr, nullptr);
	}

	void SpriteBatch::Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState, 
		std::shared_ptr<IBlendState> blendState, std::shared_ptr<IShaderProgram> shader, 
		Matrix4f& transform)
	{
		m_pImpl->Begin(spriteSortMode, &transform, blendState, samplerState, shader,
			nullptr, nullptr);
	}

	void SpriteBatch::Begin(SpriteSortMode spriteSortMode, std::shared_ptr<ISamplerState> samplerState, 
		std::shared_ptr<IBlendState> blendState, std::shared_ptr<IShaderProgram> shader, 
		Matrix4f & transform, std::shared_ptr<IRasterState> rasterState, 
		std::shared_ptr<IDepthStencilState> depthStencilState)
	{
		m_pImpl->Begin(spriteSortMode, &transform, blendState, samplerState, shader,
			rasterState, depthStencilState);
	}

	void SpriteBatch::Draw(std::shared_ptr<ITexture2D> texture, const Vector2f& center, const Color4f& color)
	{
		auto halfSize = Vector2f(texture->GetSize()) * 0.5f;
		auto minPos = center - halfSize;
		auto maxPos = center + halfSize;
		m_pImpl->Draw(texture, Bound2f(minPos, maxPos), nullptr, color, Vector2f(0.f),
			0.f, SpriteEffect::CRYSTAL_SPRITEEFFECT_NONE, 0.f);
	}

	void SpriteBatch::Draw(std::shared_ptr<ITexture2D> texture, const Bound2i& drawRect, const Color4f& color)
	{
		Bound2f rect(drawRect.GetMinPos(), drawRect.GetMaxPos());
		m_pImpl->Draw(texture, rect, nullptr, color, Vector2f(0.f), 
			0.f, SpriteEffect::CRYSTAL_SPRITEEFFECT_NONE, 0.f);
	}

	void SpriteBatch::End()
	{
		m_pImpl->End();
	}

	void SpriteBatch::Draw(std::shared_ptr<ITexture2D> texture, const Vector2f& pos,
		const Color4f& color, float rotation, const Vector2f& origin, float scale, SpriteEffect effect)
	{
		auto texScale = texture->GetSize();
		auto minPos = pos - origin * scale;
		auto maxPos = minPos + Vector2f(texScale) * scale;
		m_pImpl->Draw(texture, Bound2f(minPos, maxPos), nullptr, color,
			origin * scale, rotation, effect, 0.f);
	}


	SpriteBatch::Impl::Impl(IGraphicsDevice* graphicsDevice, IGraphicsContext* graphicsContext)
		: m_pGraphicsDevice(graphicsDevice), m_pGraphicsContext(graphicsContext)
	{
		static_assert(MAX_VERTICES_PER_BATCH < (1 << 16));

		auto assetManager = Engine::GetInstance()->GetAssetManager();

		m_pDefaultPRO = graphicsDevice->CreatePipelineResourceObject();

		// Initialize index buffers (static)
		IndexBufferDescription indexBufferDesc;
		memset(&indexBufferDesc, 0, sizeof(IndexBufferDescription));
		indexBufferDesc.Format = DataFormat::UShort16;
		indexBufferDesc.Usage = BufferUsage::Immutable;

		// 6 indices per quad
		std::unique_ptr<uint16_t[]> indices = std::make_unique<uint16_t[]>(MAX_QUADS_PER_BATCH * INDICIES_PER_QUAD);
		int startVertexOffset = 0;
		int startIndexOffset = 0;
		static uint16_t offsets[INDICIES_PER_QUAD] = { 0, 1, 3, 1, 2, 3 };
		for (int i = 0; i < MAX_QUADS_PER_BATCH; i++)
		{
			for (int j = 0; j < INDICIES_PER_QUAD; j++)
			{
				indices[startIndexOffset + j] = startVertexOffset + offsets[j];
			}
			startIndexOffset += INDICIES_PER_QUAD;
			startVertexOffset += VERTICES_PER_QUAD;
		}
		auto pIndexBuffer = m_pGraphicsDevice->CreateIndexBuffer(indexBufferDesc, &indices[0],
			sizeof(uint16_t) * MAX_QUADS_PER_BATCH * INDICIES_PER_QUAD);
		m_pDefaultPRO->SetIndexBuffer(pIndexBuffer);


		// Initialize vertex buffer (dynamic)
		VertexBufferDescription vertexBufferDesc;
		memset(&vertexBufferDesc, 0, sizeof(VertexBufferDescription));
		vertexBufferDesc.Usage = BufferUsage::CPUWrite;
		m_pDefaultVertexBuffer = m_pGraphicsDevice->CreateVertexBuffer(vertexBufferDesc, nullptr,
			sizeof(BatchVertex2D) * MAX_QUADS_PER_BATCH * VERTICES_PER_QUAD);
		std::vector<ElementDescription> elements = {
			{ SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
			{ SemanticType::TEXCOORD, 0, RenderFormat::RG32f, 12 },
			{ SemanticType::COLOR, 0, RenderFormat::RGBA32f, 20 }
		};
		VertexLayout vLayout(elements, sizeof(BatchVertex2D));
		m_pDefaultVertexBuffer->BindVertexLayout(vLayout);
		m_pDefaultPRO->SetVertexBuffer(m_pDefaultVertexBuffer);

		// Initialize pipeline states
		m_defaultRenderState.m_pRasterState = graphicsDevice->GetCommonRasterState(RasterStates::CullNone);
		m_defaultRenderState.m_pBlendState = graphicsDevice->GetCommonBlendState(BlendStates::Opaque);
		m_defaultRenderState.m_pSamplerState = graphicsDevice->GetCommonSamplerState(SamplerStates::PointClamp);

		m_defaultRenderState.m_pDepthStencilState = graphicsDevice->GetCommonDepthStencilState(DepthStencilStates::NoDepthTest);
		m_defaultRenderState.m_pShaderProgram = assetManager->LoadAsset<IShaderProgram>("Sprite");
		
		auto viewPortSize = m_pGraphicsDevice->GetContext()->GetCurrentFrameBufferSize();
		m_defaultRenderState.m_renderMatrix = glm::orthoLH_ZO(0.f, (float)viewPortSize.x, 0.f,
			(float)viewPortSize.y, -1.f, 1.f);
		m_defaultRenderState.m_spriteSortMode = SpriteSortMode::Deferred;

		m_pDefaultPSO = m_pGraphicsDevice->CreatePipelineStateObject();
	}

	SpriteBatch::Impl::~Impl()
	{}

	void SpriteBatch::Impl::Begin(SpriteSortMode spriteSortMode, 
		const Matrix4f* transform, 
		std::shared_ptr<IBlendState> blendState, 
		std::shared_ptr<ISamplerState> samplerState,
		std::shared_ptr<IShaderProgram> shader,
		std::shared_ptr<IRasterState> rasterState,
		std::shared_ptr<IDepthStencilState> depthStencilState)
	{
		if (m_isBatchingBegin)
		{
			throw std::exception("SpriteBatch::Begin cannot be called again when one has been called");
		}
		m_isBatchingBegin = true;

		if (!transform)
		{
			auto viewPortSize = m_pGraphicsDevice->GetContext()->GetCurrentFrameBufferSize();
			m_currentRenderState.m_renderMatrix = glm::orthoLH_ZO(0.f, (float)viewPortSize.x, 0.f,
				(float)viewPortSize.y, -1.f, 1.f);
		}
		else
		{
			m_currentRenderState.m_renderMatrix = *transform;
		}
		m_currentRenderState.m_pRasterState =rasterState ? rasterState : m_defaultRenderState.m_pRasterState;
		m_currentRenderState.m_pBlendState =  blendState ? blendState : m_defaultRenderState.m_pBlendState;
		m_currentRenderState.m_pDepthStencilState = depthStencilState ? depthStencilState : m_defaultRenderState.m_pDepthStencilState;
		m_currentRenderState.m_pShaderProgram = shader ? shader : m_defaultRenderState.m_pShaderProgram;
		m_currentRenderState.m_pSamplerState = samplerState ? samplerState : m_defaultRenderState.m_pSamplerState;
		m_currentRenderState.m_spriteSortMode = spriteSortMode;

		m_pDefaultPSO->SetBlendState(m_currentRenderState.m_pBlendState);
		m_pDefaultPSO->SetRasterState(m_currentRenderState.m_pRasterState);
		m_pDefaultPSO->SetDepthStencilState(m_currentRenderState.m_pDepthStencilState);

		m_pDefaultPRO->SetShaderProgram(m_currentRenderState.m_pShaderProgram);
		m_pDefaultPRO->SetSamplerState(m_currentRenderState.m_pSamplerState, 0);
	}

	void SpriteBatch::Impl::End()
	{
		if (!m_isBatchingBegin)
		{
			throw std::exception("SpriteBatch::End cannot be called if SpriteBatch::Begin() has not been called");
		}
		m_FlushThisBatch();
		m_isBatchingBegin = false;
		m_textureRefSet.clear();
	}

	void SpriteBatch::Impl::Draw(std::shared_ptr<ITexture2D> texture, const Bound2f& destRect,
					const Bound2i* srcRect, const Color4f& color, const Vector2f& origin, float rotation,
					SpriteEffect effect, float depth)
	{
		if (!texture)
		{
			throw std::invalid_argument("Texture cannot be null");
		}
		if (!m_isBatchingBegin)
		{
			throw std::logic_error("Begin must be called before Draw");
		}
		SpriteInfo spriteInfo;

		if (srcRect)
		{
			auto size = texture->GetSize();
			Vector2f minPos = srcRect->GetMinPos();
			minPos.x = minPos.x / size.x;
			minPos.y = minPos.y / size.y;
			Vector2f maxPos = srcRect->GetMaxPos();
			maxPos.x = maxPos.x / size.x;
			maxPos.y = maxPos.y / size.y;
			spriteInfo.TexCoords = Bound2f(minPos, maxPos);
		}
		else
		{
			spriteInfo.TexCoords = Bound2f(Vector2f(0.f), Vector2f(1.f));
		}

		spriteInfo.Texture = texture;
		spriteInfo.DestRect = destRect;
		spriteInfo.Origin = origin;
		spriteInfo.Color = color;
		spriteInfo.Rotation = rotation;
		spriteInfo.SpriteEffect = effect;
		spriteInfo.Depth = depth;

		if (m_currentRenderState.m_spriteSortMode == SpriteSortMode::Immediate)
		{
			m_RenderBatch(spriteInfo.Texture, &spriteInfo, 1);
		}
		else
		{
			m_spriteQueue.push_back(spriteInfo);
		}
	}

	void SpriteBatch::Impl::m_FlushThisBatch()
	{
		if (m_spriteQueue.empty()) return;
		
		int spriteCount = m_spriteQueue.size();
		int currentCount = 0;
		for (int i = 0; i < spriteCount; i++)
		{
			currentCount++;
			if (i == spriteCount - 1 || m_spriteQueue[i].Texture != m_spriteQueue[i + 1].Texture)
			{
				m_RenderBatch(m_spriteQueue[i].Texture, &m_spriteQueue[i - currentCount + 1], currentCount);
				currentCount = 0;
			}
		}
		m_spriteQueue.clear();
	}

	void SpriteBatch::Impl::m_RenderBatch(std::shared_ptr<ITexture2D> texture, const SpriteInfo* src, size_t count)
	{
		m_pDefaultPRO->SetShaderResource(texture, 0);

		m_pGraphicsContext->BeginPipeline(m_pDefaultPSO);
		{
			m_currentRenderState.m_pShaderProgram->SetUniformMat4f("MVP", m_currentRenderState.m_renderMatrix);

			m_pGraphicsContext->LoadPipelineResources(m_pDefaultPRO);
			{
				for (size_t i = 0; i < count;)
				{
					size_t quadCountThisRound = std::min(count - i, static_cast<size_t>(MAX_QUADS_PER_BATCH));
					for (int j = 0; j < quadCountThisRound; j++)
					{
						m_PushOneQuad(src[i + j]);
					}
					assert(m_vertices.size() % 4 == 0);
					size_t verticesThisRound = quadCountThisRound * VERTICES_PER_QUAD;
					m_pDefaultVertexBuffer->ChangeBufferContent(m_vertices.data(),
						verticesThisRound * sizeof(BatchVertex2D), 0);

					m_pGraphicsContext->DrawIndexedPrimitives(PrimitiveType::TRIANGLE_LIST,
							quadCountThisRound * INDICIES_PER_QUAD, 0, 0);
					m_vertices.clear();
					i += quadCountThisRound;
				}
			}
			m_pGraphicsContext->UnloadPipelineResources();
		}
		m_pGraphicsContext->EndPipeline();
	}

	void SpriteBatch::Impl::m_PushOneQuad(const SpriteInfo& sprite)
	{
		BatchVertex2D vertices[4];

		auto minCoord = sprite.TexCoords.GetMinPos();
		auto maxCoord = sprite.TexCoords.GetMaxPos();

		vertices[0].TextureCoords = minCoord;
		vertices[1].TextureCoords = Vector2f(maxCoord.x, minCoord.y);
		vertices[2].TextureCoords = maxCoord;
		vertices[3].TextureCoords = Vector2f(minCoord.x, maxCoord.y);

		if (sprite.SpriteEffect & SpriteEffect::CRYSTAL_SPRITEEFFECT_FLIP_HORIZONTAL)
		{
			std::swap(vertices[0].TextureCoords, vertices[1].TextureCoords);
			std::swap(vertices[2].TextureCoords, vertices[3].TextureCoords);
		}

		if (sprite.SpriteEffect & SpriteEffect::CRYSTAL_SPRITEEFFECT_FLIP_VERTICAL)
		{
			std::swap(vertices[0].TextureCoords, vertices[3].TextureCoords);
			std::swap(vertices[1].TextureCoords, vertices[2].TextureCoords);
		}

		auto baseScale = Vector3f(sprite.DestRect.GetSize(), 0.f);
		auto offsetPos = baseScale * 0.5f - Vector3f(sprite.Origin, 0.f);
		for (int i = 0; i < 4; i++)
		{
			vertices[i].Position = m_defaultVertices[i].Position * baseScale
				+ offsetPos;
		}

		if (sprite.Rotation != 0.f)
		{
			float cosTheta = std::cos(sprite.Rotation);
			float sinTheta = std::sin(sprite.Rotation);

			// rotate the vertices
			for (int i = 0; i < 4; i++)
			{
				float x = vertices[i].Position.x;
				float y = vertices[i].Position.y;
				vertices[i].Position.x = cosTheta * x - sinTheta * y;
				vertices[i].Position.y = sinTheta * x + cosTheta * y;
			}
		}

		if (sprite.Depth != 0)
		{
			for (int i = 0; i < 4; i++)
			{
				vertices[i].Position.z = sprite.Depth;
			}
		}

		auto originPos = sprite.DestRect.GetMinPos() + sprite.Origin;
		for (int i = 0; i < 4; i++)
		{
			vertices[i].Position.x = (int)(vertices[i].Position.x + originPos.x);
			vertices[i].Position.y = (int)(vertices[i].Position.y + originPos.y);
			vertices[i].Color = sprite.Color;

			m_vertices.push_back(vertices[i]);
		}
	}
}
