#include "TriangleTest.h"
#include <Engine.h>
#include "Platform/RHI/Interfaces.h"
#include <Core/Utils/Logger.h>
#include <Core/Utils/Geometry.h>
#include "Function/Input/InputController.h"
#include <Core/Utils/GameTimer.h>

#include <Core/Utils/Misc.h>
#include <Core/Utils/ObjLoader.h>

namespace crystal
{
    TriangleTest::TriangleTest()
    {

    }
    
    
    TriangleTest::~TriangleTest()
    {
        crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "Triangle Test destruct");
    }

    struct Vertex
    {
        Vector3f pos;
        Vector4f color;
    };

    static int indices;
    void TriangleTest::Initialize()
    {
        crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "Triangle Test Initialize");

        auto window = m_engine->GetWindow();
        auto windowSize = window->GetWindowSize();

        Vertex vertices[] =
        {
            { Vector3f(0.0f, 0.5f, 0.5f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f) },
            { Vector3f(0.5f, -0.5f, 0.5f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f) },
            { Vector3f(-0.5f, -0.5f, 0.5f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
        };
        int indices[] = 
        {
            0, 1, 2
        };

        std::vector<ElementDescription> elements = {
            { SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
            { SemanticType::COLOR, 0, RenderFormat::RGBA32f, 12 },
        };
        VertexLayout vLayout(elements, sizeof(Vertex));
        auto graphicsDevice = m_engine->GetGraphicsDevice();

        VertexBufferDescription bufferDesc{};
        bufferDesc.Usage = BufferUsage::Immutable;
        IndexBufferDescription ibufferDesc{};
        ibufferDesc.Usage = BufferUsage::Immutable;
        ibufferDesc.Format = DataFormat::UInt32;

        m_PSO = graphicsDevice->CreatePipelineStateObject();
        m_PRO = graphicsDevice->CreatePipelineResourceObject();
        auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc, 
            vertices, sizeof(vertices));
        auto indexBuffer = graphicsDevice->CreateIndexBuffer(ibufferDesc, 
            indices, sizeof(indices));

        //m_pShader = graphicsDevice->CreateShaderProgramFromFile("resources/triangle.json");

        vertexBuffer->BindVertexLayout(vLayout);
        m_PRO->SetIndexBuffer(indexBuffer);
        m_PRO->SetVertexBuffer(vertexBuffer);
        m_PRO->SetShaderProgram(m_pShader);
        //indexBuffer->Bind(0);

        m_PSO->SetBlendState(graphicsDevice->CreateBlendStateFromTemplate(BlendStates::Opaque));
        DepthStencilStateDescription DSSDesc = {};
        DSSDesc.EnableDepthTest = false;
        DSSDesc.EnableStencilTest = false;
        m_PSO->SetDepthStencilState(graphicsDevice->CreateDepthStencilState(DSSDesc));

        RasterStateDescription RSDesc = {};
        RSDesc.CullMode = CullingMode::None;
        RSDesc.FillMode = FillMode::SOLID;
        m_PSO->SetRasterState(graphicsDevice->CreateRasterState(RSDesc));
    }

    void TriangleTest::Update(const crystal::GameTimer& gameTimer)
    {
    }

    void TriangleTest::Draw(const crystal::GameTimer& gameTimer)
    {
        auto windowSize = m_engine->GetWindow()->GetWindowSize();
        auto graphicsContext = m_engine->GetGraphicsContext();
        graphicsContext->Clear(
            crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
            | crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
            | crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
            crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);
        m_pShader->SetUniform1f("uLuminance", 0.5f + 0.5f * std::sin(gameTimer.GetLogicTime()));
        m_pShader->SetUniform1f("uBase", 0.0f);

        graphicsContext->LoadPipelineState(m_PSO);

        graphicsContext->LoadPipelineResources(m_PRO);
        {
            graphicsContext->DrawIndexedPrimitives(PrimitiveType::TRIANGLE_LIST, 3, 0, 0);
        }
        graphicsContext->UnloadPipelineResources();
    }

    void TriangleTest::Exit()
    {
        crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "Triangle Test exit");
    }

    bool TriangleTest::Paused()
    {
        return m_renderPause;
    }
}