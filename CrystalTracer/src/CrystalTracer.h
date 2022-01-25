#pragma once
#include <CrystalEngine/src/Core/Application.h>
#include <CrystalEngine/src/Crystal.h>

namespace crystal
{
	class CrystalTracer : public Application
	{
	public:
		CrystalTracer(Engine* engine);
		~CrystalTracer();

		void Run();
	};
}
