#include "CrystalTracer.h"
#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Core/Platform/Platforms.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
#include <CrystalEngine/src/Core/Input/InputController.h>

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
	void CrystalTracer::Update(double deltaTime)
	{
		auto inputController = _engine->GetInstance()->GetInputController();
		if (inputController->IsKeyJustPressed(crystal::KeyCode::CRYSTAL_A_KEY))
		{
			printf("A pressed\n");
		}
	}
	void CrystalTracer::Draw(double deltaTime)
	{}
	void CrystalTracer::Exit()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer exit");
	}
	bool CrystalTracer::Paused()
	{
		return false;
	}
}