#include <Core/Render/SpriteBatch.h>
#include <Platforms/CurrentGraphics.h>

#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP


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
		Bound2f			TexCoords;
		Vector2f		Position;
		Vector2f		Origin;
		Vector2f		Scale;
		Color4f			Color;
		float			Depth;
		float			Rotation;
		Texture2D*		Texture;
		SpriteEffect	SpriteEffect;
	};

	struct RenderState
	{
		SpriteSortMode							m_spriteSortMode = SpriteSortMode::Deferred;
		Matrix4f								m_renderMatrix{};
		std::shared_ptr<PipelineStateObject>	m_pPipelineStateObject = nullptr;
		std::shared_ptr<ShaderProgram>			m_pShaderProgram = nullptr;
	};

	class SpriteBatch::Impl
	{
	public:
		explicit Impl(IGraphicsDevice* graphicsDevice);
		~Impl();
		 

		void Begin(SpriteSortMode spriteSortMode, const Matrix4f* transform,
			std::shared_ptr<PipelineStateObject> pso, std::shared_ptr<ShaderProgram> shader);
		void End();

		void Draw(std::shared_ptr<Texture2D> texture, const Vector2f& pos,
					const Bound2i* srcRect, const Color4f& color, float rotation, const Vector2f& origin,
					const Vector2f& scale, SpriteEffect effect, float depth);
	private:
		std::shared_ptr<VertexBuffer>		m_pDefaultVertexBuffer = nullptr;
		IGraphicsDevice*					m_pGraphicsDevice = nullptr;

		static constexpr int MAX_QUADS_PER_BATCH = 1 << 12;
		static constexpr int MAX_VERTICES_PER_BATCH = MAX_QUADS_PER_BATCH << 2;

		RenderState						m_defaultRenderState{};
		RenderState						m_currentRenderState{};
		std::vector<BatchVertex2D>		m_vertices{};
		std::vector<SpriteInfo>			m_spriteQueue{};
		bool							m_isBatchingBegin = false;

		static BatchVertex2D			m_defaultVertices[4];

		void m_FlushThisBatch();
		void m_RenderBatch(Texture2D* texture, const SpriteInfo* src, size_t count);
		void m_UpdateOneQuad();
		void m_CheckFlush();
		void m_PushOneVertex(const BatchVertex2D& vertex);
	};

	BatchVertex2D SpriteBatch::Impl::m_defaultVertices[] =
	{
		{ Vector3f(-0.5f, -0.5f, 0.f), Vector2f(0.0f, 0.0f), Vector4f(1.0f) },
		{ Vector3f(0.5f, -0.5f, 0.f), Vector2f(1.0f, 0.0f), Vector4f(1.0f) },
		{ Vector3f(0.5f, 0.5f, 0.f), Vector2f(1.0f, 1.0f), Vector4f(1.0f) },
		{ Vector3f(-0.5f, 0.5f, 0.f), Vector2f(0.0f, 1.0f), Vector4f(1.0f) },
	};

	SpriteBatch::SpriteBatch(IGraphicsDevice* graphicsDevice)
	{
		m_pImpl = std::make_unique<Impl>(graphicsDevice);
	}

	SpriteBatch::~SpriteBatch()
	{}

	void SpriteBatch::PushBatch(const Matrix4f& transform)
	{
		m_FlushThisBatch();
		auto renderState = RenderState(transform, m_pDefaultRenderState->m_pPipelineStateObject, m_globalQuadIndex);
		m_renderStateStack.push_back(renderState);
	}

	void SpriteBatch::PopBatch()
	{
		m_FlushThisBatch();
		m_renderStateStack.pop_back();
	}

	void SpriteBatch::Draw(std::shared_ptr<Texture2D> texture, const Vector2f& center, const Color4f& color)
	{
		m_CheckFlush();
		int textureSlot = m_GetTextureSlot(texture);
		auto halfSize = Vector3f(texture->GetSize(), 0.f) * 0.5f;
		for (int i = 0; i < 4; i++)
		{
			m_PushOneVertex(BatchVertex2D{ Vector3f(center, 0.f) + m_defaultVertices[i].Position * halfSize,
				m_defaultVertices[i].TextureCoords, color, (float)textureSlot });
		}
		m_UpdateOneQuad();
	}

	void SpriteBatch::Draw(std::shared_ptr<Texture2D> texture, const Bound2i& drawRect, const Color4f& color)
	{
		m_CheckFlush();
		int textureSlot = m_GetTextureSlot(texture);
		auto minPos = drawRect.GetMinPos();
		auto maxPos = drawRect.GetMaxPos();

		m_PushOneVertex(BatchVertex2D{ Vector3f(minPos, 0.f), m_defaultVertices[0].TextureCoords,
				color, (float)textureSlot });
		m_PushOneVertex(BatchVertex2D{ Vector3f(maxPos.x, minPos.y, 0.f), m_defaultVertices[1].TextureCoords,
			color, (float)textureSlot });
		m_PushOneVertex(BatchVertex2D{ Vector3f(maxPos, 0.f), m_defaultVertices[2].TextureCoords,
			color, (float)textureSlot });
		m_PushOneVertex(BatchVertex2D{ Vector3f(minPos.x, maxPos.y, 0.f), m_defaultVertices[3].TextureCoords,
			color, (float)textureSlot });

		m_UpdateOneQuad();
	}

	void SpriteBatch::Draw(std::shared_ptr<Texture2D> texture, const Vector2f& pos, const Color4f& color, float rotation, const Vector2f& origin, float scale, SpriteEffect effect)
	{
		m_GeneralDraw(texture, pos, Bound2i(Vector2i(0.f), Vector2i(texture->GetSize())),
			color, rotation, origin, Vector2f(scale), effect, 0.f);
	}

	int SpriteBatch::m_GetTextureSlot(std::shared_ptr<Texture2D> texture)
	{
		auto& currentState = m_renderStateStack.back();
		auto it = currentState.m_textureSlotMap.find(ptr(texture));
		int textureSlot = 0;
		if (it != currentState.m_textureSlotMap.end())
		{
			textureSlot = it->second;
		}
		else
		{
			// If too much distinct textures then flush batches
			if (currentState.m_currentTextureSlotCount == MAX_TEXTURE_SLOTS)
			{
				m_FlushThisBatch();
				currentState.m_textureSlotMap.clear();
				currentState.m_currentTextureSlotCount = 0;
			}
			textureSlot = currentState.m_currentTextureSlotCount;
			currentState.m_pPipelineStateObject->BindShaderResource(texture, textureSlot);
			currentState.m_textureSlotMap[ptr(texture)] = textureSlot;
			currentState.m_currentTextureSlotCount++;
		}
		return textureSlot;
	}

	void SpriteBatch::m_FlushThisBatch()
	{
		if (m_renderStateStack.empty()) return;
		auto& currentState = m_renderStateStack.back();
		if (currentState.m_currentQuadCount == 0) return;
		assert(currentState.m_currentQuadCount == m_globalQuadIndex - currentState.m_currentQuadOffset);
		m_globalQuadIndex = currentState.m_currentQuadOffset;
		m_currentVertexIndex = m_globalQuadIndex * 4;
		{
			m_pGraphicsDevice->PushPipelineStateObject(currentState.m_pPipelineStateObject ?
				currentState.m_pPipelineStateObject : m_pDefaultRenderState->m_pPipelineStateObject);

			assert(m_vertices.size() % 4 == 0);

			auto& currentShader = currentState.m_pShaderProgram ? currentState.m_pShaderProgram : m_pDefaultShaderProgram;
			currentShader->SetUniformMat4f("MVP", currentState.m_renderMatrix);
			currentShader->Apply();

			size_t totalQuadsThisRound = currentState.m_currentQuadCount;
			size_t verticesThisRound = totalQuadsThisRound * 4;
			m_pDefaultVertexBuffer->ChangeBufferContent(m_vertices.data() + currentState.m_currentQuadOffset * 4,
				verticesThisRound * sizeof(BatchVertex2D), currentState.m_currentQuadOffset * 4);

			m_pGraphicsDevice->DrawIndexedPrimitives(PrimitiveType::TRIANGLE_LIST,
				totalQuadsThisRound * 6, currentState.m_currentQuadOffset * 6, 0);

			m_pGraphicsDevice->PopPipelineStateObject();
		}
		currentState.m_currentQuadCount = 0;
	}

	void SpriteBatch::m_UpdateOneQuad()
	{
		m_globalQuadIndex++;
		auto& currentState = m_renderStateStack.back();
		currentState.m_currentQuadCount++;
	}

	void SpriteBatch::m_CheckFlush()
	{
		if (m_globalQuadIndex == MAX_QUADS_PER_BATCH)
		{
			m_FlushThisBatch();
		}
	}

	void SpriteBatch::m_PushOneVertex(const BatchVertex2D& vertex)
	{
		if (m_currentVertexIndex == m_vertices.size())
		{
			m_vertices.push_back(vertex);
		}
		else
		{
			m_vertices[m_currentVertexIndex] = vertex;
		}
		m_currentVertexIndex++;
	}

	void SpriteBatch::m_GeneralDraw(std::shared_ptr<Texture2D> texture, const Vector2f& pos,
		const Bound2i& srcRect, const Color4f& color, float rotation, const Vector2f& origin,
		const Vector2f& scale, SpriteEffect effect, float depth)
	{
		m_CheckFlush();
		int textureSlot = m_GetTextureSlot(texture);
		Vector2f texSize = Vector2f(texture->GetSize());

		BatchVertex2D vertices[4];

		auto srcRectMin = srcRect.GetMinPos();
		auto srcRectMax = srcRect.GetMaxPos();

		vertices[0].TextureCoords.x = srcRectMin.x / texSize.x;
		vertices[0].TextureCoords.y = srcRectMin.y / texSize.y;

		vertices[1].TextureCoords.x = srcRectMax.x / texSize.x;
		vertices[1].TextureCoords.y = srcRectMin.y / texSize.y;

		vertices[2].TextureCoords.x = srcRectMax.x / texSize.x;
		vertices[2].TextureCoords.y = srcRectMax.y / texSize.y;

		vertices[3].TextureCoords.x = srcRectMin.x / texSize.x;
		vertices[3].TextureCoords.y = srcRectMax.y / texSize.y;


		if (effect & SpriteEffect::CRYSTAL_SPRITEEFFECT_FLIP_HORIZONTAL)
		{
			std::swap(vertices[0].TextureCoords, vertices[1].TextureCoords);
			std::swap(vertices[2].TextureCoords, vertices[3].TextureCoords);
		}

		if (effect & SpriteEffect::CRYSTAL_SPRITEEFFECT_FLIP_VERTICAL)
		{
			std::swap(vertices[0].TextureCoords, vertices[3].TextureCoords);
			std::swap(vertices[1].TextureCoords, vertices[2].TextureCoords);
		}

		for (int i = 0; i < 4; i++)
		{
			vertices[i].Position = m_defaultVertices[i].Position * Vector3f(texSize, 0.f)
				+ Vector3f(-origin + texSize * 0.5f, 0.f);
			vertices[i].Color = color;
		}

		if (rotation != 0.f)
		{
			float cosTheta = std::cos(rotation);
			float sinTheta = std::sin(rotation);

			// rotate the vertices
			for (int i = 0; i < 4; i++)
			{
				float x = vertices[i].Position.x;
				float y = vertices[i].Position.y;
				vertices[i].Position.x = cosTheta * x - sinTheta * y;
				vertices[i].Position.y = sinTheta * x + cosTheta * y;
			}
		}

		if (depth != 0)
		{
			for (int i = 0; i < 4; i++)
			{
				vertices[i].Position.z = depth;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			vertices[i].Position.x = vertices[i].Position.x * scale.x + pos.x;
			vertices[i].Position.y = vertices[i].Position.y * scale.y + pos.y;

			m_PushOneVertex(vertices[i]);
		}
		m_UpdateOneQuad();
	}

	SpriteBatch::Impl::Impl(IGraphicsDevice* graphicsDevice)
		: m_pGraphicsDevice(graphicsDevice)
	{
		static_assert(MAX_VERTICES_PER_BATCH < (1 << 16));

		auto& defaultPSO = m_currentRenderState.m_pPipelineStateObject;
		defaultPSO = m_pGraphicsDevice->CreatePipelineStateObject();
		defaultPSO->SetDepthTestState(false);
		defaultPSO->BindSamplerState(graphicsDevice->GetSamplerState(SamplerStates::PointClamp), 0);
		

		// Initialize index buffers (static)
		IndexBufferDescription indexBufferDesc;
		memset(&indexBufferDesc, 0, sizeof(IndexBufferDescription));
		indexBufferDesc.Format = DataFormat::UShort16;
		indexBufferDesc.Usage = BufferUsage::Immutable;

		// 6 indices per quad
		std::unique_ptr<uint32_t[]> indices = std::make_unique<uint32_t[]>(MAX_QUADS_PER_BATCH * INDICIES_PER_QUAD);
		int startVertexOffset = 0;
		int startIndexOffset = 0;
		static uint32_t offsets[INDICIES_PER_QUAD] = { 0, 1, 3, 1, 2, 3 };
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
			sizeof(uint32_t) * MAX_QUADS_PER_BATCH * INDICIES_PER_QUAD);
		defaultPSO->BindIndexBuffer(pIndexBuffer);


		// Initialize vertex buffer (dynamic)
		VertexBufferDescription vertexBufferDesc;
		memset(&vertexBufferDesc, 0, sizeof(VertexBufferDescription));
		vertexBufferDesc.Usage = BufferUsage::CPUWrite;
		auto vertexBuffer = m_pGraphicsDevice->CreateVertexBuffer(vertexBufferDesc, nullptr,
			sizeof(BatchVertex2D) * MAX_QUADS_PER_BATCH * VERTICES_PER_QUAD);
		std::vector<ElementDescription> elements = {
			{ SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
			{ SemanticType::TEXCOORD, 0, RenderFormat::RG32f, 12 },
			{ SemanticType::COLOR, 0, RenderFormat::RGBA32f, 20 }
		};
		VertexLayout vLayout(elements, sizeof(BatchVertex2D));
		vertexBuffer->BindVertexLayout(vLayout);
		defaultPSO->BindVertexBuffer(vertexBuffer);
		m_pDefaultVertexBuffer = vertexBuffer;

		m_currentRenderState.m_pShaderProgram = m_pGraphicsDevice->CreateShaderProgramFromFile("resources/sprite.json");
		m_currentRenderState.m_renderMatrix = glm::identity<Matrix4f>();
		m_currentRenderState.m_spriteSortMode = SpriteSortMode::Deferred;
	}

	SpriteBatch::Impl::~Impl()
	{}

	void SpriteBatch::Impl::Begin(SpriteSortMode spriteSortMode, const Matrix4f* transform,
			std::shared_ptr<PipelineStateObject> pso, std::shared_ptr<ShaderProgram> shader)
	{
		if (m_isBatchingBegin)
		{
			throw std::exception("SpriteBatch::Begin cannot be called again when one has been called");
		}
		m_isBatchingBegin = true;

		m_currentRenderState.m_renderMatrix = transform ? *transform : m_defaultRenderState.m_renderMatrix;
		m_currentRenderState.m_pPipelineStateObject =  pso ? pso : m_defaultRenderState.m_pPipelineStateObject;
		m_currentRenderState.m_pShaderProgram = shader ? shader : m_defaultRenderState.m_pShaderProgram;
		m_currentRenderState.m_spriteSortMode = spriteSortMode;
	}

	void SpriteBatch::Impl::End()
	{
		if (!m_isBatchingBegin)
		{
			throw std::exception("SpriteBatch::End cannot be called if SpriteBatch::Begin() has not been called");
		}
		m_isBatchingBegin = false;
	}

	void SpriteBatch::Impl::Draw(std::shared_ptr<Texture2D> texture, const Vector2f& pos,
					const Bound2i* srcRect, const Color4f& color, float rotation, const Vector2f& origin,
					const Vector2f& scale, SpriteEffect effect, float depth)
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

		spriteInfo.Texture = texture.get();
		spriteInfo.Position = pos;
		spriteInfo.Origin = origin;
		spriteInfo.Scale = scale;
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
		if (m_vertices.empty()) return;
		{
			m_pGraphicsDevice->PushPipelineStateObject(m_currentRenderState.m_pPipelineStateObject);
			
			m_pGraphicsDevice->PopPipelineStateObject();
		}
	}

	void SpriteBatch::Impl::m_RenderBatch(Texture2D* texture, const SpriteInfo* src, size_t count)
	{
		
	}
}
