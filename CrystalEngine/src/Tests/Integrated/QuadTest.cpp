#include "QuadTest.h"
#include <Core/Utils/Logger.h>
#include <Core/Utils/Geometry.h>
#include <Core/Input/InputController.h>
#include <Core/Utils/GameTimer.h>
#include <Core/Render/SpriteBatch.h>

#include <Core/Utils/Misc.h>
#include <Core/Utils/ObjLoader.h>

namespace crystal
{
	QuadTest::QuadTest()
	{}

	QuadTest::~QuadTest()
	{}

	struct Vertex
	{
		Vector2f	pos;
		Vector2f	texCoord;
		Vector4f	color;
		float		index;
	};

	static Vertex vertices[] =
	{
		{ Vector2f(-0.5f, -0.5f), Vector2f(0.0f, 0.0f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f), 0.f },
		{ Vector2f(0.5f, -0.5f), Vector2f(1.0f, 0.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f), 0.f },
		{ Vector2f(0.5f, 0.5f), Vector2f(1.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f), 0.f },
		{ Vector2f(-0.5f, 0.5f), Vector2f(0.0f, 1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f), 0.f },
	};
	static int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	static std::shared_ptr<IVertexBuffer> vertexBuffer;

	void QuadTest::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer initialize");

		auto window = m_engine->GetWindow();
		auto windowSize = window->GetWindowSize();
		auto graphicsDevice = m_engine->GetGraphicsDevice();

		//std::vector<ElementDescription> elements = {
		//	{ SemanticType::POSITION, 0, RenderFormat::RG32f, 0 },
		//	{ SemanticType::TEXCOORD, 0, RenderFormat::RG32f, 8 },
		//	{ SemanticType::COLOR, 0, RenderFormat::RGBA32f, 16 },
		//	{ SemanticType::POSITION, 1, RenderFormat::R32f, 32 },
		//};
		//VertexLayout vLayout(elements, sizeof(Vertex));

		//VertexBufferDescription bufferDesc{};
		//bufferDesc.Usage = BufferUsage::CPUWrite;
		//IndexBufferDescription indexDesc;
		//indexDesc.Format = DataFormat::UInt32;
		//indexDesc.Usage = BufferUsage::Immutable;
		//auto indexBuffer = graphicsDevice->CreateIndexBuffer(indexDesc, indices, sizeof(indices));
		//vertexBuffer = graphicsDevice->CreateVertexBuffer(bufferDesc, nullptr, sizeof(vertices));
		//vertexBuffer->BindVertexLayout(vLayout);
		//m_PSO->BindVertexBuffer(vertexBuffer);
		//m_PSO->BindIndexBuffer(indexBuffer);


		//m_pShader = graphicsDevice->CreateShaderProgramFromFile("resources/sprite.json");
		//m_PSO->BindShaderProgram(m_pShader);

		//m_PSO->SetCullMode(CullingMode::CullCW);

		Texture2DDescription texturedesc;
		texturedesc.Format = RenderFormat::RGBA8ub;
		texturedesc.MipmapLevels = 1;
		texturedesc.Usage = BufferUsage::Default;

		m_texture2D = graphicsDevice->CreateTexture2DFromFile("resources/test.jpg", texturedesc);
		m_texture2D1 = graphicsDevice->CreateTexture2DFromFile("resources/dots.png", texturedesc);

        m_PSO = graphicsDevice->CreatePipelineStateObject();
        m_PSO->SetRasterState(graphicsDevice->CreateRasterStateFromTemplate(RasterStates::CullNone));
        m_PSO->SetBlendState(graphicsDevice->CreateBlendStateFromTemplate(BlendStates::AlphaBlend));
        m_PSO->SetDepthStencilState(graphicsDevice->CreateDepthStencilStateFromTemplate(DepthStencilStates::NoDepthTest));
		//m_PSO->BindShaderResource(m_texture2D, 0);
		//m_PSO->BindSamplerState(graphicsDevice->GetSamplerState(SamplerStates::PointClamp), 0);
	}

	void QuadTest::Update(const crystal::GameTimer& gameTimer)
	{}

	static float R = 0.f;
	void QuadTest::Draw(const crystal::GameTimer& gameTimer)
	{
		auto windowSize = m_engine->GetWindow()->GetWindowSize();
		auto graphicsContext = m_engine->GetGraphicsContext();
		auto graphicsDevice = m_engine->GetGraphicsDevice();
		auto spriteBatch = m_engine->GetSpriteBatch();
		graphicsContext->Clear(
			crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
			| crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
			| crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
			crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);

		R += 0.01;
		auto size = m_texture2D->GetSize();
		//spriteBatch->Begin();
		//spriteBatch->Draw(m_texture2D, Vector2f(200.f, 300.f), Color4f(1.f),
		//			R, Vector2f(size) * 0.5f, 4.f, SpriteEffect::CRYSTAL_SPRITEEFFECT_NONE);
		//spriteBatch->End();

		//spriteBatch->Begin(SpriteSortMode::Deferred, 
		//	graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp));
		//spriteBatch->Draw(m_texture2D, Vector2f(600.f, 300.f), Color4f(1.f),
		//	R, Vector2f(size) * 0.5f, 4.f, SpriteEffect::CRYSTAL_SPRITEEFFECT_NONE);
		//spriteBatch->End();

		spriteBatch->Begin(SpriteSortMode::Deferred, m_PSO,
			graphicsDevice->GetCommonSamplerState(SamplerStates::LinearClamp));
		spriteBatch->Draw(m_texture2D, Vector2f(300.f, 300.f), Color4f(1.f, 1.f, 1.f, 0.5f),
			0.f, Vector2f(size) * 0.5f, 5.f, SpriteEffect::CRYSTAL_SPRITEEFFECT_NONE);
		spriteBatch->Draw(m_texture2D, Vector2f(400.f, 350.f), Color4f(1.f, 1.f, 1.f, 0.5f),
			0.f, Vector2f(size) * 0.5f, 5.f, SpriteEffect::CRYSTAL_SPRITEEFFECT_NONE);
		spriteBatch->End();
	}

	void QuadTest::Exit()
	{}

	bool QuadTest::Paused()
	{
		return false;
	}
}
