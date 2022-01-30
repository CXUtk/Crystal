#include "CrystalTracer.h"
#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Core/Platform/Platforms.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
#include <CrystalEngine/src/Core/Utils/Geometry.h>
#include <CrystalEngine/src/Core/Input/InputController.h>
#include <CrystalEngine/src/Core/Utils/GameTimer.h>

using namespace crystal;

namespace tracer
{
	CrystalTracer::CrystalTracer()
	{

	}
	
	
	CrystalTracer::~CrystalTracer()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer destruct");
	}

	struct Vertex
	{
		Vector3f	Position;
		Color4f		Color;
	};

	void CrystalTracer::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer initialize");
		Vertex vertices[] =
		{
			{ Vector3f(0.0f, 0.5f, 0.5f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vector3f(0.5f, -0.5f, 0.5f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Vector3f(-0.5f, -0.5f, 0.5f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f) },
		};
		std::vector<ElementDescription> elements = {
			{ SemanticType::POSITION, 0, VertexElementFormat::Vector3, 0 },
			{ SemanticType::COLOR, 0, VertexElementFormat::Vector4, 12 },
		};
		VertexLayout vLayout(elements);
		auto graphicsDevice = m_engine->GetGraphicsDevice();

		BufferDescription bufferDesc;
		bufferDesc.Type = BufferType::ARRAY_BUFFER;
		bufferDesc.Usage = BufferUsage::IMMUTABLE_DRAW;
		auto vertexBuffer = graphicsDevice->CreateBuffer(bufferDesc, vertices, sizeof(vertices));

		vertexBuffer->BindVertexLayout(vLayout);

	}
	void CrystalTracer::Update(const crystal::GameTimer& gameTimer)
	{
		auto inputController = m_engine->GetInstance()->GetInputController();
		if (inputController->IsKeyJustPressed(crystal::KeyCode::CRYSTAL_A_KEY))
		{
			m_renderPause = !m_renderPause;
			printf("%s\n", m_renderPause ? "Paused" : "Resumed");
		}
		if (inputController->IsKeyJustPressed(crystal::KeyCode::CRYSTAL_S_KEY))
		{
			printf("Time: %lf\n", gameTimer.GetLogicTime());
			printf("Time: %lf\n", gameTimer.GetLogicalDeltaTime());
		}
	}
	void CrystalTracer::Draw(const crystal::GameTimer& gameTimer)
	{
		auto graphicsDevice = m_engine->GetGraphicsDevice();
		graphicsDevice->Clear(
			crystal::ClearOptions::Target 
			| crystal::ClearOptions::Stencil
			| crystal::ClearOptions::Depth,
			crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);

	}
	void CrystalTracer::Exit()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer exit");
	}
	bool CrystalTracer::Paused()
	{
		return m_renderPause;
	}
}