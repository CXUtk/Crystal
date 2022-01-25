#pragma once
#include <CrystalEngine/src/Core/Application.h>
#include <CrystalEngine/src/Crystal.h>
namespace tracer
{
	class CrystalTracer : public crystal::Application
	{
	public:
		CrystalTracer();
		virtual ~CrystalTracer() override;

		virtual void Initialize() override;

		virtual void Update(double deltaTime) override;

		virtual void Draw(double deltaTime) override;
	};
}
