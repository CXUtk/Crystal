#include "SpriteBatch.h"
#include <Core/Platform/Platforms.h>
#include <Platforms/CurrentGraphics.h>

#undef min
#undef max

namespace crystal
{
	static BatchVertex2D defaultVertices[] =
	{
		{ Vector2f(-0.5f, -0.5f), Vector2f(0.0f, 0.0f), Vector4f(0.0f), 0.0f },
		{ Vector2f(0.5f, -0.5f), Vector2f(1.0f, 0.0f), Vector4f(0.0f), 0.0f },
		{ Vector2f(0.5f, 0.5f), Vector2f(1.0f, 1.0f), Vector4f(0.0f), 0.0f },
		{ Vector2f(-0.5f, 0.5f), Vector2f(0.0f, 1.0f), Vector4f(0.0f), 0.0f },
	};

	SpriteBatch::SpriteBatch(IGraphicsDevice* graphicsDevice)
		: m_pGraphicsDevice(graphicsDevice)
	{
		m_pPipelineStateObject = m_pGraphicsDevice->CreatePipelineStateObject();
		m_pPipelineStateObject->SetDepthTestState(false);

		// Initialize index buffers (static)
		IndexBufferDescription indexBufferDesc;
		memset(&indexBufferDesc, 0, sizeof(IndexBufferDescription));
		indexBufferDesc.Format = DataFormat::UInt32;
		indexBufferDesc.Usage = BufferUsage::Immutable;

		// 6 indices per quad
		std::unique_ptr<uint32_t[]> indices = std::make_unique<uint32_t[]>(MAX_QUADS_PER_BATCH * 6);
		int startVertexOffset = 0;
		int startIndexOffset = 0;
		static uint32_t offsets[6] = {0, 1, 3, 1, 2, 3};
		for (int i = 0; i < MAX_QUADS_PER_BATCH; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				indices[startIndexOffset + j] = startIndexOffset + offsets[j];
			}
			startIndexOffset += 6;
			startVertexOffset += 4;
		}

		auto pIndexBuffer = m_pGraphicsDevice->CreateIndexBuffer(indexBufferDesc, &indices[0],
			sizeof(uint32_t) * MAX_QUADS_PER_BATCH * 6);
		m_pPipelineStateObject->BindIndexBuffer(pIndexBuffer);


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
		m_pPipelineStateObject->BindVertexBuffer(vertexBuffer);

		m_pVertexBuffer = vertexBuffer;

		m_pShaderProgram = m_pGraphicsDevice->CreateShaderProgramFromFile("resources/sprite.json");
	}

	SpriteBatch::~SpriteBatch()
	{}

	void SpriteBatch::Begin(const Matrix4f& transform)
	{
		if (m_batchHasBegin)
		{
			throw std::exception("SpriteBatch::Begin cannot be called when one is already began");
		}

		m_renderMatrix = transform;
		m_currentQuadIndex = 0;
		m_currentTextureSlot = 0;
		m_vertices.clear();
		m_textureSlotMap.clear();

		m_batchHasBegin = true;
	}

	void SpriteBatch::End()
	{
		if (!m_batchHasBegin)
		{
			throw std::exception("SpriteBatch::End cannot be called when begin was not called");
		}
		m_batchHasBegin = false;

		m_pGraphicsDevice->PushPipelineStateObject(m_pPipelineStateObject);
		{
			assert(m_vertices.size() % 4 == 0);
			
			m_pShaderProgram->SetUniformMat4f("MVP", m_renderMatrix);
			m_pShaderProgram->Apply();

			size_t totalQuads = m_vertices.size() / 4;
			for (size_t i = 0; i < totalQuads;)
			{
				size_t verticesThisRound = std::min(totalQuads - i, (size_t)MAX_QUADS_PER_BATCH) * 4;
				m_pVertexBuffer->ChangeBufferContent(m_vertices.data(),
					verticesThisRound * sizeof(BatchVertex2D), 0);

				m_pGraphicsDevice->DrawIndexedPrimitives(PrimitiveType::TRIANGLE_LIST,
					verticesThisRound, i * 6, 0);
				i += verticesThisRound;
			}
		}
		m_pGraphicsDevice->PopPipelineStateObject();
	}

	void SpriteBatch::Draw(Texture2D* texture, const Vector2f& center)
	{
		auto it = m_textureSlotMap.find(texture);
		int textureSlot = 0;
		if (it != m_textureSlotMap.end())
		{
			textureSlot = it->second;
		}
		else
		{
			textureSlot = m_currentTextureSlot;
			m_textureSlotMap[texture] = m_currentTextureSlot;
			m_currentTextureSlot++;
		}
		
		auto halfSize = Vector2f(texture->GetSize()) * 0.5f;
		for (int i = 0; i < 4; i++)
		{
			m_vertices.push_back(BatchVertex2D{ center + defaultVertices[i].Position * halfSize });
		}
		m_currentQuadIndex++;
	}
}
