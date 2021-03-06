#include "SpriteBatch.h"
#include "Platform/RHI/Graphics/GraphicsCommon.h"
#include <stdexcept>
#include <Engine.h>
#include "Resource/Asset/AssetManager.h"
#include "Resource/Asset/Content/Fonts/Font.h"


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
        Bound2f							TexCoords{};
        Bound2f							DestRect{};
        Color4f							Color{};
        Vector2f						Origin{};
        float							Depth = 0.f;
        float							Rotation = 0.f;
        std::shared_ptr<ITexture2D>		Texture = nullptr;
        SpriteEffect					SpriteEffect = SpriteEffect::None;
    };

    struct RenderStateSB
    {
        SpriteSortMode							m_spriteSortMode = SpriteSortMode::Deferred;
        Matrix4f								m_renderMatrix{};
        std::shared_ptr<IPipelineStateObject>   m_PSO = nullptr;
        std::shared_ptr<IShaderProgram>			m_pShaderProgram = nullptr;
        std::shared_ptr<ISamplerState>			m_pSamplerState = nullptr;
    };

    //inline RenderState::~RenderState()
    //{
    //    printf("23333\n");
    //}

    class SpriteBatch::Impl
    {
    public:
        explicit Impl(IGraphicsDevice* graphicsDevice, IGraphicsContext* graphicsContext);
        ~Impl();


        void Begin(SpriteSortMode spriteSortMode,
            const Matrix4f* transform,
            std::shared_ptr<IPipelineStateObject> pso,
            std::shared_ptr<ISamplerState> samplerState,
            std::shared_ptr<IShaderProgram> shader);
        void End();

        void Draw(std::shared_ptr<ITexture2D> texture, const Bound2f& destRect,
                    const Bound2i* srcRect, const Color4f& color, const Vector2f& origin, float rotation,
                    SpriteEffect effect, float depth);

    private:
        static constexpr int MAX_QUADS_PER_BATCH = 1 << 12;
        static constexpr int MAX_VERTICES_PER_BATCH = MAX_QUADS_PER_BATCH << 2;

        IGraphicsDevice* m_pGraphicsDevice = nullptr;
        IGraphicsContext* m_pGraphicsContext = nullptr;

        std::shared_ptr<IPipelineResourceObject>	m_pDefaultPRO = nullptr;
        std::shared_ptr<IVertexBuffer>				m_pDefaultVertexBuffer = nullptr;

        RenderStateSB					m_defaultRenderState{};
        RenderStateSB					m_currentRenderState{};
        std::vector<BatchVertex2D>		m_vertices{};
        std::vector<SpriteInfo>			m_spriteQueue{};
        std::shared_ptr<ITexture2D>     m_whiteTexture = nullptr;
        bool							m_isBatchingBegin = false;

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
        m_pImpl = std::make_unique<SpriteBatch::Impl>(graphicsDevice, graphicsContext);
    }

    SpriteBatch::~SpriteBatch()
    {}

    void SpriteBatch::Begin()
    {
        m_pImpl->Begin(SpriteSortMode::Deferred, nullptr, nullptr, nullptr, nullptr);
    }

    void SpriteBatch::Begin(SpriteSortMode spriteSortMode,
            std::shared_ptr<IPipelineStateObject> PSO)
    {
        m_pImpl->Begin(spriteSortMode, nullptr, PSO, nullptr, nullptr);
    }

    void SpriteBatch::Begin(SpriteSortMode spriteSortMode,
            std::shared_ptr<IPipelineStateObject> PSO,
            std::shared_ptr<ISamplerState> samplerState)
    {
        m_pImpl->Begin(spriteSortMode, nullptr, PSO, samplerState, nullptr);
    }

    void SpriteBatch::Begin(SpriteSortMode spriteSortMode,
            std::shared_ptr<IPipelineStateObject> PSO,
            std::shared_ptr<ISamplerState> samplerState,
            std::shared_ptr<IShaderProgram> shader)
    {
        m_pImpl->Begin(spriteSortMode, nullptr, PSO, samplerState, shader);
    }

    void SpriteBatch::Begin(SpriteSortMode spriteSortMode,
            std::shared_ptr<IPipelineStateObject> PSO,
            std::shared_ptr<ISamplerState> samplerState,
            std::shared_ptr<IShaderProgram> shader,
            const Matrix4f& transform)
    {
        m_pImpl->Begin(spriteSortMode, &transform, PSO, samplerState, shader);
    }

    void SpriteBatch::Draw(std::shared_ptr<ITexture2D> texture, const Vector2f& center, const Color4f& color)
    {
        auto halfSize = Vector2f(texture->GetSize()) * 0.5f;
        auto minPos = center - halfSize;
        auto maxPos = center + halfSize;
        m_pImpl->Draw(texture, Bound2f(minPos, maxPos), nullptr, color, Vector2f(0.f),
            0.f, SpriteEffect::None, 0.f);
    }

    void SpriteBatch::Draw(std::shared_ptr<ITexture2D> texture, const Bound2i& drawRect, const Color4f& color)
    {
        Bound2f rect(drawRect.GetMinPos(), drawRect.GetMaxPos());
        m_pImpl->Draw(texture, rect, nullptr, color, Vector2f(0.f),
            0.f, SpriteEffect::None, 0.f);
    }

    void SpriteBatch::Draw(std::shared_ptr<ITexture2D> texture, const Bound2i& drawRect, const Bound2i* srcRect, const Color4f& color)
    {
        Bound2f rect(drawRect.GetMinPos(), drawRect.GetMaxPos());
        m_pImpl->Draw(texture, rect, srcRect, color, Vector2f(0.f),
            0.f, SpriteEffect::None, 0.f);
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

    void SpriteBatch::DrawSlicedTexture(std::shared_ptr<ITexture2D> texture, const SliceInfo& sliceInfo, const Bound2i& screenBound, const Color4f& color)
    {
        auto texSize = texture->GetSize();
        auto minPos = screenBound.GetMinPos();
        auto maxPos = screenBound.GetMaxPos();
        auto boundSize = maxPos - minPos;
        if (sliceInfo.Bot > 0)
        {
            if ((sliceInfo.DrawFlags & 1) && sliceInfo.Left > 0)
            {
                auto botLeft = Bound2i(Vector2i(0), Vector2i(sliceInfo.Left, sliceInfo.Bot));
                auto texBotLeft = Bound2i(Vector2i(0), Vector2i(sliceInfo.Left, sliceInfo.Bot));
                Draw(texture, minPos + botLeft, &texBotLeft, color);
            }

            if ((sliceInfo.DrawFlags & (1 << 1)) && sliceInfo.Left + sliceInfo.Right < boundSize.x)
            {
                auto botMid = Bound2i(Vector2i(sliceInfo.Left, 0), Vector2i(boundSize.x - sliceInfo.Right, sliceInfo.Bot));
                auto texBotMid = Bound2i(Vector2i(sliceInfo.Left, 0), Vector2i(texSize.x - sliceInfo.Right, sliceInfo.Bot));
                Draw(texture, minPos + botMid, &texBotMid, color);
            }

            if ((sliceInfo.DrawFlags & (1 << 2)) && sliceInfo.Right > 0)
            {
                auto botRight = Bound2i(Vector2i(boundSize.x - sliceInfo.Right, 0), Vector2i(boundSize.x, sliceInfo.Bot));
                auto texBotRight = Bound2i(Vector2i(texSize.x - sliceInfo.Right, 0), Vector2i(texSize.x, sliceInfo.Bot));
                Draw(texture, minPos + botRight, &texBotRight, color);
            }
        }
        if (sliceInfo.Top + sliceInfo.Bot < boundSize.y)
        {
            if ((sliceInfo.DrawFlags & (1 << 3)) && sliceInfo.Left > 0)
            {
                auto midLeft = Bound2i(Vector2i(0, sliceInfo.Bot), Vector2i(sliceInfo.Left, boundSize.y - sliceInfo.Top));
                auto texMidLeft = Bound2i(Vector2i(0, sliceInfo.Bot), Vector2i(sliceInfo.Left, texSize.y - sliceInfo.Top));
                Draw(texture, minPos + midLeft, &texMidLeft, color);
            }

            if ((sliceInfo.DrawFlags & (1 << 4)) && sliceInfo.Left + sliceInfo.Right < boundSize.x)
            {
                auto midMid = Bound2i(Vector2i(sliceInfo.Left, sliceInfo.Bot), Vector2i(boundSize.x - sliceInfo.Right, boundSize.y - sliceInfo.Top));
                auto texMidMid = Bound2i(Vector2i(sliceInfo.Left, sliceInfo.Bot), Vector2i(texSize.x - sliceInfo.Right, texSize.y - sliceInfo.Top));
                Draw(texture, minPos + midMid, &texMidMid, color);
            }

            if ((sliceInfo.DrawFlags & (1 << 5)) && sliceInfo.Right > 0)
            {
                auto midRight = Bound2i(Vector2i(boundSize.x - sliceInfo.Right, sliceInfo.Bot), Vector2i(boundSize.x, boundSize.y - sliceInfo.Top));
                auto texMidRight = Bound2i(Vector2i(texSize.x - sliceInfo.Right, sliceInfo.Bot), Vector2i(texSize.x, texSize.y - sliceInfo.Top));
                Draw(texture, minPos + midRight, &texMidRight, color);
            }
        }

        if (sliceInfo.Top > 0)
        {
            if ((sliceInfo.DrawFlags & (1 << 6)) && sliceInfo.Left > 0)
            {
                auto topLeft = Bound2i(Vector2i(0, boundSize.y - sliceInfo.Top), Vector2i(sliceInfo.Left, boundSize.y));
                auto texTopLeft = Bound2i(Vector2i(0, texSize.y - sliceInfo.Top), Vector2i(sliceInfo.Left, texSize.y));
                Draw(texture, minPos + topLeft, &texTopLeft, color);
            }

            if ((sliceInfo.DrawFlags & (1 << 7)) && sliceInfo.Left + sliceInfo.Right < boundSize.x)
            {
                auto topMid = Bound2i(Vector2i(sliceInfo.Left, boundSize.y - sliceInfo.Top), Vector2i(boundSize.x - sliceInfo.Right, boundSize.y));
                auto texTopMid = Bound2i(Vector2i(sliceInfo.Left, texSize.y - sliceInfo.Top), Vector2i(texSize.x - sliceInfo.Right, texSize.y));
                Draw(texture, minPos + topMid, &texTopMid, color);
            }

            if ((sliceInfo.DrawFlags & (1 << 8)) && sliceInfo.Right > 0)
            {
                auto topRight = Bound2i(Vector2i(boundSize.x - sliceInfo.Right, boundSize.y - sliceInfo.Top), Vector2i(boundSize.x, boundSize.y));
                auto texTopRight = Bound2i(Vector2i(texSize.x - sliceInfo.Right, texSize.y - sliceInfo.Top), Vector2i(texSize.x, texSize.y));
                Draw(texture, minPos + topRight, &texTopRight, color);
            }
        }
    }

    void SpriteBatch::DrawString(std::shared_ptr<Font> font, const std::string& text, const Vector2f& pos, const Color4f& color)
    {
        Vector2f origin = pos;
        auto codes = font->GetCodesFromString(text);
        for (auto c : codes)
        {
            auto& character = font->GetCharacter(c);
            if (character.Texture2D)
            {
                auto texSize = Vector2f(character.Size);
                auto minPos = origin + Vector2f(character.Bearing.x, character.Bearing.y - texSize.y);
                auto maxPos = minPos + texSize;
                m_pImpl->Draw(character.Texture2D, Bound2f(minPos, maxPos), nullptr, color,
                    Vector2f(0.f), 0.f, SpriteEffect::None, 0.f);
            }
            origin.x += character.Advance;
        }
    }

    void SpriteBatch::DrawString(std::shared_ptr<Font> font, const std::u32string_view& text, const Vector2f& pos, const Color4f& color)
    {
        Vector2f origin = pos;
        for (auto c : text)
        {
            auto& character = font->GetCharacter(c);
            if (character.Texture2D)
            {
                auto texSize = Vector2f(character.Size);
                auto minPos = origin + Vector2f(character.Bearing.x, character.Bearing.y - texSize.y);
                auto maxPos = minPos + texSize;
                m_pImpl->Draw(character.Texture2D, Bound2f(minPos, maxPos), nullptr, color,
                    Vector2f(0.f), 0.f, SpriteEffect::None, 0.f);
            }
            origin.x += character.Advance;
        }
    }


    SpriteBatch::Impl::Impl(IGraphicsDevice* graphicsDevice, IGraphicsContext* graphicsContext)
        : m_pGraphicsDevice(graphicsDevice), m_pGraphicsContext(graphicsContext)
    {
        static_assert(MAX_VERTICES_PER_BATCH < (1 << 16));

        auto assetManager = Engine::GetInstance()->GetAssetManager();

        m_pDefaultPRO = m_pGraphicsDevice->CreatePipelineResourceObject();
        m_defaultRenderState.m_PSO = m_pGraphicsDevice->CreatePipelineStateObject();

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
        auto& defPSO = m_defaultRenderState.m_PSO;
        defPSO->SetRasterState(m_pGraphicsDevice->CreateRasterStateFromTemplate(RasterStates::CullNone));
        defPSO->SetBlendState(m_pGraphicsDevice->CreateBlendStateFromTemplate(BlendStates::Opaque));
        defPSO->SetDepthStencilState(m_pGraphicsDevice->CreateDepthStencilStateFromTemplate(DepthStencilStates::NoDepthTest));

        m_defaultRenderState.m_pSamplerState = m_pGraphicsDevice->GetCommonSamplerState(SamplerStates::PointClamp);
        m_defaultRenderState.m_pShaderProgram = assetManager->LoadAssetBuiltIn<IShaderProgram>("Sprite");

        auto viewPortSize = m_pGraphicsDevice->GetContext()->GetCurrentFrameBufferSize();
        m_defaultRenderState.m_renderMatrix = glm::orthoLH_ZO(0.f, (float)viewPortSize.x, 0.f,
            (float)viewPortSize.y, -1.f, 1.f);
        m_defaultRenderState.m_spriteSortMode = SpriteSortMode::Deferred;
    }

    SpriteBatch::Impl::~Impl()
    {}

    void SpriteBatch::Impl::Begin(SpriteSortMode spriteSortMode,
        const Matrix4f* transform,
        std::shared_ptr<IPipelineStateObject> pso,
        std::shared_ptr<ISamplerState> samplerState,
        std::shared_ptr<IShaderProgram> shader)
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
        m_currentRenderState.m_PSO = pso ? pso : m_defaultRenderState.m_PSO;
        m_currentRenderState.m_pShaderProgram = shader ? shader : m_defaultRenderState.m_pShaderProgram;
        m_currentRenderState.m_pSamplerState = samplerState ? samplerState : m_defaultRenderState.m_pSamplerState;
        m_currentRenderState.m_spriteSortMode = spriteSortMode;

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
        m_currentRenderState.m_pShaderProgram->SetUniformMat4f("MVP", m_currentRenderState.m_renderMatrix);

        m_pGraphicsContext->LoadPipelineState(m_currentRenderState.m_PSO);
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

    void SpriteBatch::Impl::m_PushOneQuad(const SpriteInfo& sprite)
    {
        BatchVertex2D vertices[4];

        auto minCoord = sprite.TexCoords.GetMinPos();
        auto maxCoord = sprite.TexCoords.GetMaxPos();

        vertices[0].TextureCoords = minCoord;
        vertices[1].TextureCoords = Vector2f(maxCoord.x, minCoord.y);
        vertices[2].TextureCoords = maxCoord;
        vertices[3].TextureCoords = Vector2f(minCoord.x, maxCoord.y);

        if (sprite.SpriteEffect & SpriteEffect::FlipHorizontal)
        {
            std::swap(vertices[0].TextureCoords, vertices[1].TextureCoords);
            std::swap(vertices[2].TextureCoords, vertices[3].TextureCoords);
        }

        if (sprite.SpriteEffect & SpriteEffect::FlipVertical)
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
