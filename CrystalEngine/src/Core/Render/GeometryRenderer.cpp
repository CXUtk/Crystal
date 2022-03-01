#include "GeometryRenderer.h"

#include <stdexcept>

#include <Interfaces/Interfaces.h>
#include <Engine.h>
#include <Core/Asset/AssetManager.h>

namespace crystal
{
    constexpr size_t MAX_VERTICES = 1 << 16;
    struct BatchGVertex2D
    {
        Vector3f	Position;
        Color4f		Color;
    };

    struct BatchGeometryCommand
    {
        size_t          NumVertices;
        PrimitiveType   PrimitiveType;
    };

    struct RenderState
    {
        Matrix4f								m_renderMatrix{};
        std::shared_ptr<IShaderProgram>			m_pShaderProgram = nullptr;
        std::shared_ptr<IRasterState>			m_pRasterState = nullptr;
        std::shared_ptr<IBlendState>			m_pBlendState = nullptr;
        std::shared_ptr<IDepthStencilState>		m_pDepthStencilState = nullptr;
    };

    class GeometryRenderer::Impl
    {
    public:
        explicit Impl(IGraphicsDevice* graphicsDevice, IGraphicsContext* context);
        ~Impl();

        void Begin(const Matrix4f* transform,
            std::shared_ptr<IBlendState> blendState,
            std::shared_ptr<IShaderProgram> shader,
            std::shared_ptr<IRasterState> rasterState,
            std::shared_ptr<IDepthStencilState> depthStencilState);
        void End();

        void BeginNewPrimitive(PrimitiveType type);
        void AppendGVertex(BatchGVertex2D&& vertex);
        void AppendGVertex(const BatchGVertex2D& vertex);
    private:
        IGraphicsDevice*    m_pGraphicsDevice = nullptr;
        IGraphicsContext*   m_pGraphicsContext = nullptr;

        RenderState			m_defaultRenderState{};
        RenderState			m_currentRenderState{};

        std::shared_ptr<IPipelineStateObject>		m_pDefaultPSO = nullptr;
        std::shared_ptr<IPipelineResourceObject>	m_pDefaultPRO = nullptr;
        std::shared_ptr<IVertexBuffer>				m_pDefaultVertexBuffer = nullptr;

        std::vector<BatchGeometryCommand>   m_commandList{};
        std::vector<BatchGVertex2D>		    m_vertices{};
        bool							    m_isBatchingBegin = false;

        void FlushThisBatch();
    };

    GeometryRenderer::Impl::Impl(IGraphicsDevice* graphicsDevice, IGraphicsContext* context)
        : m_pGraphicsDevice(graphicsDevice), m_pGraphicsContext(context)
    {
        auto assetManager = Engine::GetInstance()->GetAssetManager();

        m_pDefaultPRO = m_pGraphicsDevice->CreatePipelineResourceObject();
        m_pDefaultPSO = m_pGraphicsDevice->CreatePipelineStateObject();

        // Initialize vertex buffer (dynamic)
        VertexBufferDescription vertexBufferDesc;
        memset(&vertexBufferDesc, 0, sizeof(VertexBufferDescription));
        vertexBufferDesc.Usage = BufferUsage::CPUWrite;
        m_pDefaultVertexBuffer = m_pGraphicsDevice->CreateVertexBuffer(vertexBufferDesc, nullptr,
            sizeof(BatchGVertex2D) * MAX_VERTICES);
        std::vector<ElementDescription> elements = {
            { SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
            { SemanticType::COLOR, 0, RenderFormat::RGBA32f, 12 }
        };
        VertexLayout vLayout(elements, sizeof(BatchGVertex2D));
        m_pDefaultVertexBuffer->BindVertexLayout(vLayout);
        m_pDefaultPRO->SetVertexBuffer(m_pDefaultVertexBuffer);

        // Initialize pipeline states
        m_defaultRenderState.m_pRasterState = m_pGraphicsDevice->GetCommonRasterState(RasterStates::CullNone);
        m_defaultRenderState.m_pBlendState = m_pGraphicsDevice->GetCommonBlendState(BlendStates::Opaque);
        m_defaultRenderState.m_pDepthStencilState = m_pGraphicsDevice->GetCommonDepthStencilState(DepthStencilStates::NoDepthTest);
        m_defaultRenderState.m_pShaderProgram = assetManager->LoadAsset<IShaderProgram>("package1:Geometry");
    }

    GeometryRenderer::Impl::~Impl()
    {

    }

    void GeometryRenderer::Impl::Begin(const Matrix4f* transform,
         std::shared_ptr<IBlendState> blendState,
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
            auto viewPortSize = m_pGraphicsContext->GetCurrentFrameBufferSize();
            m_currentRenderState.m_renderMatrix = glm::orthoLH_ZO(0.f, (float)viewPortSize.x, 0.f,
                (float)viewPortSize.y, -1.f, 1.f);
        }
        else
        {
            m_currentRenderState.m_renderMatrix = *transform;
        }
        m_currentRenderState.m_pRasterState = rasterState ? rasterState : m_defaultRenderState.m_pRasterState;
        m_currentRenderState.m_pBlendState = blendState ? blendState : m_defaultRenderState.m_pBlendState;
        m_currentRenderState.m_pDepthStencilState = depthStencilState ? depthStencilState : m_defaultRenderState.m_pDepthStencilState;
        m_currentRenderState.m_pShaderProgram = shader ? shader : m_defaultRenderState.m_pShaderProgram;

        m_pDefaultPSO->SetBlendState(m_currentRenderState.m_pBlendState);
        m_pDefaultPSO->SetRasterState(m_currentRenderState.m_pRasterState);
        m_pDefaultPSO->SetDepthStencilState(m_currentRenderState.m_pDepthStencilState);

        m_pDefaultPRO->SetShaderProgram(m_currentRenderState.m_pShaderProgram);

        m_currentRenderState.m_pShaderProgram->SetUniformMat4f("MVP", m_currentRenderState.m_renderMatrix);
    }

    void GeometryRenderer::Impl::End()
    {
        if (!m_isBatchingBegin)
        {
            throw std::exception("SpriteBatch::End cannot be called if SpriteBatch::Begin() has not been called");
        }
        FlushThisBatch();
        m_isBatchingBegin = false;
    }

    void GeometryRenderer::Impl::BeginNewPrimitive(PrimitiveType type)
    {
        if (m_commandList.empty() || m_commandList.back().PrimitiveType != type
            || (m_commandList.back().PrimitiveType == PrimitiveType::LINE_STRIP
                || m_commandList.back().PrimitiveType == PrimitiveType::TRIANGLE_STRIP))
        {
            BatchGeometryCommand command = {};
            command.PrimitiveType = type;
            command.NumVertices = 0;
            m_commandList.push_back(std::move(command));
        }
    }

    void GeometryRenderer::Impl::AppendGVertex(BatchGVertex2D&& vertex)
    {
        m_vertices.push_back(std::forward<BatchGVertex2D>(vertex));
        m_commandList.back().NumVertices++;
    }

    void GeometryRenderer::Impl::AppendGVertex(const BatchGVertex2D& vertex)
    {
        m_vertices.push_back(vertex);
        m_commandList.back().NumVertices++;
    }

    void GeometryRenderer::Impl::FlushThisBatch()
    {
        m_pGraphicsContext->BeginPipeline(m_pDefaultPSO);
        {
            m_pGraphicsContext->LoadPipelineResources(m_pDefaultPRO);
            {
                for (size_t i = 0; i < m_vertices.size(); i += MAX_VERTICES)
                {
                    size_t verticesThisRound = std::min(MAX_VERTICES, m_vertices.size() - i);
                    m_pDefaultVertexBuffer->ChangeBufferContent(m_vertices.data(),
                        verticesThisRound * sizeof(BatchGVertex2D), 0);

                    size_t offset = 0;
                    for (auto& command : m_commandList)
                    {
                        m_pGraphicsContext->DrawPrimitives(
                            command.PrimitiveType,
                            offset,
                            command.NumVertices
                        );
                        offset += command.NumVertices;
                    }
                }

            }
            m_pGraphicsContext->UnloadPipelineResources();
        }
        m_pGraphicsContext->EndPipeline();

        m_vertices.clear();
        m_commandList.clear();
    }

    GeometryRenderer::GeometryRenderer(IGraphicsDevice* graphicsDevice, IGraphicsContext* context)
    {
        m_pImpl = std::make_unique<GeometryRenderer::Impl>(graphicsDevice, context);
    }

    GeometryRenderer::~GeometryRenderer()
    {}

    void GeometryRenderer::Begin()
    {
        m_pImpl->Begin(nullptr, nullptr, nullptr, nullptr, nullptr);
    }

    void GeometryRenderer::End()
    {
        m_pImpl->End();
    }


    void GeometryRenderer::DrawLineStrip(const std::vector<Vector2f>& points, const Color4f& color)
    {
        m_pImpl->BeginNewPrimitive(PrimitiveType::LINE_STRIP);
        for (auto& v : points)
        {
            BatchGVertex2D vertex = {};
            vertex.Color = color;
            vertex.Position.x = v.x;
            vertex.Position.y = v.y;
            m_pImpl->AppendGVertex(std::move(vertex));
        }
    }


    void GeometryRenderer::DrawLine(const Vector2f& start, const Vector2f& end, const Color4f& color)
    {
        m_pImpl->BeginNewPrimitive(PrimitiveType::LINE_LIST);

        BatchGVertex2D vertex = {};
        vertex.Color = color;
        vertex.Position.x = start.x;
        vertex.Position.y = start.y;
        m_pImpl->AppendGVertex(vertex);

        vertex.Position.x = end.x;
        vertex.Position.y = end.y;
        m_pImpl->AppendGVertex(vertex);
    }

    void GeometryRenderer::DrawLine(const Vector3f& start, const Vector3f& end, const Color4f& color)
    {
        m_pImpl->BeginNewPrimitive(PrimitiveType::LINE_LIST);

        BatchGVertex2D vertex = {};
        vertex.Color = color;
        vertex.Position = start;
        m_pImpl->AppendGVertex(vertex);

        vertex.Position = end;
        m_pImpl->AppendGVertex(vertex);
    }

    void GeometryRenderer::DrawTriangle(const Vector3f& V1, const Vector3f& V2, const Vector3f& V3, const Color4f& color)
    {
        m_pImpl->BeginNewPrimitive(PrimitiveType::TRIANGLE_LIST);

        BatchGVertex2D vertex = {};
        vertex.Color = color;
        vertex.Position = V1;
        m_pImpl->AppendGVertex(vertex);

        vertex.Position = V2;
        m_pImpl->AppendGVertex(vertex);

        vertex.Position = V3;
        m_pImpl->AppendGVertex(vertex);
    }

    void GeometryRenderer::DrawBound2D(const Bound2i& bound, const Color4f& color)
    {
        m_pImpl->BeginNewPrimitive(PrimitiveType::LINE_STRIP);

        auto minPos = bound.GetMinPos();
        auto maxPos = bound.GetMaxPos();

        BatchGVertex2D vertex = {};
        vertex.Color = color;
        vertex.Position = Vector3f(minPos.x, minPos.y, 0.f);
        m_pImpl->AppendGVertex(vertex);

        vertex.Position = Vector3f(maxPos.x, minPos.y, 0.f);
        m_pImpl->AppendGVertex(vertex);

        vertex.Position = Vector3f(maxPos.x, maxPos.y, 0.f);
        m_pImpl->AppendGVertex(vertex);

        vertex.Position = Vector3f(minPos.x, maxPos.y, 0.f);
        m_pImpl->AppendGVertex(vertex);
    }
}