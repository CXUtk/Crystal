#include "SpriteBatch.h"
#include <Core/Platform/Platforms.h>
#include <Platforms/CurrentGraphics.h>

#undef min
#undef max

namespace crystal
{
	class SpriteBatch::RenderState
	{
	public:
		RenderState() : m_renderMatrix(glm::identity<Matrix4f>())
		{

		}
		RenderState(const Matrix4f& transform, std::shared_ptr<PipelineStateObject> pso, 
			size_t currentQuadOffset)
			: m_renderMatrix(transform), m_pPipelineStateObject(pso),
			m_currentQuadOffset(currentQuadOffset)
		{

		}
		~RenderState() {}

		Matrix4f						m_renderMatrix{};
		// May be interface?
		std::shared_ptr<PipelineStateObject>	m_pPipelineStateObject = nullptr;
		std::shared_ptr<IShaderProgram>			m_pShaderProgram = nullptr;

		size_t							m_currentQuadCount = 0;
		size_t							m_currentQuadOffset = 0;
		std::map<Texture2D*, int>		m_textureSlotMap{};
		size_t							m_currentTextureSlotCount = 0;
	};

	static BatchVertex2D defaultVertices[] =
	{
		{ Vector2f(-0.5f, -0.5f), Vector2f(0.0f, 0.0f), Vector4f(1.0f), 0.0f },
		{ Vector2f(0.5f, -0.5f), Vector2f(1.0f, 0.0f), Vector4f(1.0f), 0.0f },
		{ Vector2f(0.5f, 0.5f), Vector2f(1.0f, 1.0f), Vector4f(1.0f), 0.0f },
		{ Vector2f(-0.5f, 0.5f), Vector2f(0.0f, 1.0f), Vector4f(1.0f), 0.0f },
	};


	SpriteBatch::SpriteBatch(IGraphicsDevice* graphicsDevice)
		: m_pGraphicsDevice(graphicsDevice)
	{
		std::shared_ptr<PipelineStateObject> defaultPSO = m_pGraphicsDevice->CreatePipelineStateObject();
		defaultPSO->SetDepthTestState(false);
		for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
		{
			defaultPSO->BindSamplerState(SamplerState::GetSamplerState(SamplerStates::LinearClamp), i);
		}

		// Initialize index buffers (static)
		IndexBufferDescription indexBufferDesc;
		memset(&indexBufferDesc, 0, sizeof(IndexBufferDescription));
		indexBufferDesc.Format = DataFormat::UInt32;
		indexBufferDesc.Usage = BufferUsage::Immutable;

		// 6 indices per quad
		std::unique_ptr<uint32_t[]> indices = std::make_unique<uint32_t[]>(MAX_QUADS_PER_BATCH * 6);
		int startVertexOffset = 0;
		int startIndexOffset = 0;
		static uint32_t offsets[6] = { 0, 1, 3, 1, 2, 3 };
		for (int i = 0; i < MAX_QUADS_PER_BATCH; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				indices[startIndexOffset + j] = startVertexOffset + offsets[j];
			}
			startIndexOffset += 6;
			startVertexOffset += 4;
		}

		auto pIndexBuffer = m_pGraphicsDevice->CreateIndexBuffer(indexBufferDesc, &indices[0],
			sizeof(uint32_t) * MAX_QUADS_PER_BATCH * 6);
		defaultPSO->BindIndexBuffer(pIndexBuffer);


		// Initialize vertex buffer (dynamic)
		VertexBufferDescription vertexBufferDesc;
		memset(&vertexBufferDesc, 0, sizeof(VertexBufferDescription));
		vertexBufferDesc.Usage = BufferUsage::CPUWrite;

		auto vertexBuffer = m_pGraphicsDevice->CreateVertexBuffer(vertexBufferDesc, nullptr,
			sizeof(BatchVertex2D) * MAX_QUADS_PER_BATCH * 4);

		std::vector<ElementDescription> elements = {
			{ SemanticType::POSITION, 0, RenderFormat::RG32f, 0 },
			{ SemanticType::TEXCOORD, 0, RenderFormat::RG32f, 8 },
			{ SemanticType::COLOR, 0, RenderFormat::RGBA32f, 16 },
			{ SemanticType::POSITION, 1, RenderFormat::R32f, 32 },
		};
		VertexLayout vLayout(elements, sizeof(BatchVertex2D));
		vertexBuffer->BindVertexLayout(vLayout);
		defaultPSO->BindVertexBuffer(vertexBuffer);

		m_pDefaultVertexBuffer = vertexBuffer;
		m_pDefaultShaderProgram = m_pGraphicsDevice->CreateShaderProgramFromFile("resources/sprite.json");
		m_pDefaultRenderState = std::make_shared<RenderState>(glm::identity<Matrix4f>(), defaultPSO, 0);
	}

	SpriteBatch::~SpriteBatch()
	{}

	void SpriteBatch::PushBatch(const Matrix4f & transform)
	{
		auto renderState = RenderState(transform, m_pDefaultRenderState->m_pPipelineStateObject, m_globalQuadIndex);
		m_renderStateStack.push_back(renderState);
	}

	void SpriteBatch::PopBatch()
	{
		m_FlushThisBatch();
		auto& currentState = m_renderStateStack.back();
		assert(currentState.m_currentQuadCount == m_globalQuadIndex - currentState.m_currentQuadOffset);
		m_globalQuadIndex = currentState.m_currentQuadOffset;
		m_currentVertexIndex = m_globalQuadIndex * 4;
		m_renderStateStack.pop_back();
	}

	void SpriteBatch::Draw(std::shared_ptr<Texture2D> texture, const Vector2f& center, const Color4f& color)
	{
		m_CheckFlush();
		int textureSlot = m_GetTextureSlot(texture);
		auto halfSize = Vector2f(texture->GetSize()) * 0.5f;
		for (int i = 0; i < 4; i++)
		{
			m_PushOneVertex(BatchVertex2D{ center + defaultVertices[i].Position * halfSize, defaultVertices[i].TextureCoords,
				color, (float)textureSlot });
		}
		m_UpdateOneQuad();
	}

	void SpriteBatch::Draw(std::shared_ptr<Texture2D> texture, const Bound2i& drawRect, const Color4f& color)
	{
		m_CheckFlush();
		int textureSlot = m_GetTextureSlot(texture);
		auto minPos = drawRect.GetMinPos();
		auto maxPos = drawRect.GetMaxPos();

		m_PushOneVertex(BatchVertex2D{ minPos, defaultVertices[0].TextureCoords,
				color, (float)textureSlot });
		m_PushOneVertex(BatchVertex2D{ Vector2f(maxPos.x, minPos.y), defaultVertices[1].TextureCoords,
			color, (float)textureSlot });
		m_PushOneVertex(BatchVertex2D{ maxPos, defaultVertices[2].TextureCoords,
			color, (float)textureSlot });
		m_PushOneVertex(BatchVertex2D{ Vector2f(minPos.x, maxPos.y), defaultVertices[3].TextureCoords,
			color, (float)textureSlot });

		m_UpdateOneQuad();
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
		auto& currentState = m_renderStateStack.back();
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
}
