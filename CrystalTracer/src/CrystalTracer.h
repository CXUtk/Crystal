#pragma once
#include <CrystalEngine/src/Core/Application.h>
#include <CrystalEngine/src/Crystal.h>

namespace crystal
{
	class CrystalTracer : public Application
	{
	public:
		CrystalTracer();
		virtual ~CrystalTracer() override;

		virtual void Initialize() override;

		virtual void Update(double deltaTime) override;

		virtual void Draw(double deltaTime) override;
	};
}
