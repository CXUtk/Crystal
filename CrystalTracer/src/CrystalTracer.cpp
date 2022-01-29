#include "CrystalTracer.h"
#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Core/Platform/Platforms.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
#include <CrystalEngine/src/Core/Input/InputController.h>
#include <CrystalEngine/src/Core/Utils/GameTimer.h>

namespace tracer
{
	CrystalTracer::CrystalTracer()
	{

	}
	CrystalTracer::~CrystalTracer()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer destruct");
	}
	void CrystalTracer::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer initialize");
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
	{}
	void CrystalTracer::Exit()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer exit");
	}
	bool CrystalTracer::Paused()
	{
		return m_renderPause;
	}
}