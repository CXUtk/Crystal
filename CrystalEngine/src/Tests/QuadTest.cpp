#include "QuadTest.h"
#include <Core/Platform/Platforms.h>
#include <Core/Utils/Logger.h>
#include <Core/Utils/Geometry.h>
#include <Core/Input/InputController.h>
#include <Core/Utils/GameTimer.h>

#include <Core/Utils/Misc.h>
#include <Core/Utils/ObjLoader.h>

#ifdef CRYSTAL_USE_OPENGL
#include <Platforms/OpenGL/OpenGLPlatform.h>
#elif defined(CRYSTAL_USE_DX11)
#include <Platforms/DX11/DX11Platform.h>
#endif

namespace crystal
{
	QuadTest::QuadTest()
	{}

	QuadTest::~QuadTest()
	{}

	struct Vertex
	{
		Vector3f pos;
		Vector2f texCoord;
		Vector4f color;
	};

	void QuadTest::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer initialize");

		auto window = m_engine->GetWindow();
		auto windowSize = window->GetWindowSize();
		auto graphicsDevice = m_engine->GetGraphicsDevice();

		Vertex vertices[] =
		{
			{ Vector3f(-0.5f, -0.5f, 0.5f), Vector2f(0.0f, 0.0f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3f(0.5f, -0.5f, 0.5f), Vector2f(1.0f, 0.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Vector3f(0.5f, 0.5f, 0.5f), Vector2f(1.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vector3f(-0.5f, 0.5f, 0.5f), Vector2f(0.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
		};
		int indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		std::vector<ElementDescription> elements = {
			{ SemanticType::POSITION, 0, RenderFormat::RGB32f, 0 },
			{ SemanticType::TEXCOORD, 0, RenderFormat::RGB32f, 12 },
			{ SemanticType::COLOR, 0, RenderFormat::RGBA32f, 24 },
		};
		VertexLayout vLayout(elements, sizeof(Vertex));

		VertexBufferDescription bufferDesc{};
		bufferDesc.Usage = BufferUsage::Immutable;
		IndexBufferDescription indexDesc;
		indexDesc.Format = DataFormat::UInt32;
		indexDesc.Usage = BufferUsage::Immutable;
		auto indexBuffer = graphicsDevice->CreateIndexBuffer(indexDesc, indices, sizeof(indices));
		auto vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc, vertices, sizeof(vertices));
		vertexBuffer->BindVertexLayout(vLayout);

		m_PSO = graphicsDevice->CreatePipelineStateObject();
		m_PSO->BindVertexBuffer(vertexBuffer);
		m_PSO->BindIndexBuffer(indexBuffer);


		m_pShader = graphicsDevice->CreateShaderProgramFromFile("resources/sprite.json");
		m_PSO->BindShaderProgram(m_pShader);

		m_PSO->SetCullMode(CullingMode::CullCW);

		Texture2DDescription texturedesc;
		texturedesc.Format = RenderFormat::RGBA8ub;
		texturedesc.MipmapLevels = 1;
		texturedesc.Usage = BufferUsage::Default;

		m_texture2D = graphicsDevice->CreateTexture2D("resources/dots.png", texturedesc);
		m_PSO->BindShaderResource(m_texture2D, 0);
		m_PSO->BindSamplerState(SamplerState::GetSamplerState(SamplerStates::LinearClamp), 0);
	}

	void QuadTest::Update(const crystal::GameTimer& gameTimer)
	{}

	void QuadTest::Draw(const crystal::GameTimer & gameTimer)
	{
		auto windowSize = m_engine->GetWindow()->GetWindowSize();
		auto graphicsDevice = m_engine->GetGraphicsDevice();
		graphicsDevice->Clear(
			crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
			| crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
			| crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
			crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);

		graphicsDevice->SetPipelineStateObject(m_PSO);
		//graphicsDevice->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, 0, 3);
		graphicsDevice->DrawIndexedPrimitives(PrimitiveType::TRIANGLE_LIST, 6, 0, 0);
	}

	void QuadTest::Exit()
	{}

	bool QuadTest::Paused()
	{
		return false;
	}
}
